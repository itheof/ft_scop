/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wfobj.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/07 10:41:37 by tvallee           #+#    #+#             */
/*   Updated: 2018/09/10 13:42:06 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "wfobj.h"

struct	s_kw_handler
{
	char const	*kw;
	t_bool		(*handle)(t_wfobj *dst, char const *line, char const *kw);
};

static t_bool	comment_skip(t_wfobj *dst, char const *line, char const *kw)
{
	(void)kw;
	(void)dst;
	(void)line;
	return (true);
}

static t_bool	unimplemented(t_wfobj *dst, char const *line, char const *kw)
{

	(void)dst;
	(void)line;
	fprintf(stderr, "keyword %sunimplemented", kw);
	return (false);
}

static t_bool	parse_vertex(t_wfobj *dst, char const *line, char const *kw)
{
	t_vertex	vertex;
	t_list		*lst;

	(void)kw;
	vertex.ndim = sscanf(line, "%f %f %f %f", vertex.coord + COORD_X,
			vertex.coord + COORD_Y, vertex.coord + COORD_Z, vertex.coord + COORD_W);
	if (vertex.ndim < 3)
		fprintf(stderr, "unsufficient vertex input");
	else
	{
		if (vertex.ndim != 4)
			vertex.coord[COORD_W] = 1.0f;
		if (!(lst = ft_lstnew(&vertex, sizeof(vertex))))
		{
			perror("parse_vertex malloc");
			return (false);
		}
		ft_lstpushback(&dst->v, lst);
		dst->n_v++;
	}
	return (true);
}

static t_bool	parse_vertex_normal(t_wfobj *dst, char const *line,
		char const *kw)
{
	t_vertex_normal	vn;
	t_list			*lst;
	int				nscan;

	(void)kw;
	nscan = sscanf(line, "%f %f %f", vn.coord + COORD_X, vn.coord + COORD_Y,
			vn.coord + COORD_Z);
	if (nscan != 3)
	{
		fprintf(stderr, "unsufficient vertex normal input");
		return (false);
	}
	if (!(lst = ft_lstnew(&vn, sizeof(vn))))
	{
		perror("parse_vertex_normal malloc");
		return (false);
	}
	ft_lstpushback(&dst->vn, lst);
	dst->n_vn++;
	return (true);
}

static t_bool	parse_vertex_texture(t_wfobj *dst, char const *line,
		char const *kw)
{
	t_vertex_texture	vt;
	t_list				*lst;

	(void)kw;
	vt.ndim = sscanf(line, "%f %f %f", vt.coord + COORD_X, vt.coord + COORD_Y,
			vt.coord + COORD_Z);
	if (vt.ndim == 0)
	{
		fprintf(stderr, "unsufficient vertex texture input");
		return (false);
	}
	if (vt.ndim < 3)
		vt.coord[COORD_Z] = 0.0f;
	if (vt.ndim < 2)
		vt.coord[COORD_Y] = 0.0f;
	if (!(lst = ft_lstnew(&vt, sizeof(vt))))
	{
		perror("parse_vertex_texture malloc");
		return (false);
	}
	(void)kw;
	ft_lstpushback(&dst->vt, lst);
	dst->n_vt++;
	return (true);
}

static t_bool	parse_face_push(t_wfobj *dst, t_face f)
{
	t_list	*lst;

	if (!(lst = ft_lstnew(&f, sizeof(f))))
	{
		perror("parse_face_push malloc");
		return (false);
	}
	ft_lstpushback(&dst->f, lst);
	return (true);
}

/* vertices are 1-indexed /!\ */

static t_bool	parse_face(t_wfobj *dst, char const *line, char const *kw)
{
	t_face	f;

	(void)kw;
	if ((f.n_sides = sscanf(line, "%d %d %d %d", &f.triplets[0][TRIPLET_V],
					&f.triplets[1][TRIPLET_V], &f.triplets[2][TRIPLET_V],
					&f.triplets[3][TRIPLET_V])) >= 3)
	{
		f.normal = false;
		f.texture = false;
		return (parse_face_push(dst, f));
	}
	else if ((f.n_sides = sscanf(line, "%d// %d// %d// %d//",
				&f.triplets[0][TRIPLET_V], &f.triplets[1][TRIPLET_V],
				&f.triplets[2][TRIPLET_V], &f.triplets[3][TRIPLET_V])) >= 3)
	{
		f.normal = false;
		f.texture = false;
		return (parse_face_push(dst, f));
	}
	else if ((f.n_sides = sscanf(line, "%d/%d/ %d/%d/ %d/%d/ %d/%d/",
				&f.triplets[0][TRIPLET_V], &f.triplets[0][TRIPLET_VT],
				&f.triplets[1][TRIPLET_V], &f.triplets[1][TRIPLET_VT],
				&f.triplets[2][TRIPLET_V], &f.triplets[2][TRIPLET_VT],
				&f.triplets[3][TRIPLET_V], &f.triplets[3][TRIPLET_VT]
				)) >= 6)
	{
		f.texture = true;
		f.normal = false;
		return (parse_face_push(dst, f));
	}
	else if ((f.n_sides = sscanf(line, "%d//%d %d//%d %d//%d %d//%d",
				&f.triplets[0][TRIPLET_V], &f.triplets[0][TRIPLET_VN],
				&f.triplets[1][TRIPLET_V], &f.triplets[1][TRIPLET_VN],
				&f.triplets[2][TRIPLET_V], &f.triplets[2][TRIPLET_VN],
				&f.triplets[3][TRIPLET_V], &f.triplets[3][TRIPLET_VN]
				)) >= 6)
	{
		f.texture = false;
		f.normal = true;
		return (parse_face_push(dst, f));
	}
	else if ((f.n_sides = sscanf(line, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d/",
				&f.triplets[0][TRIPLET_V], &f.triplets[0][TRIPLET_VT], &f.triplets[0][TRIPLET_VN],
				&f.triplets[1][TRIPLET_V], &f.triplets[1][TRIPLET_VT], &f.triplets[1][TRIPLET_VN],
				&f.triplets[2][TRIPLET_V], &f.triplets[2][TRIPLET_VT], &f.triplets[2][TRIPLET_VN],
				&f.triplets[3][TRIPLET_V], &f.triplets[3][TRIPLET_VT], &f.triplets[3][TRIPLET_VN]
				)) >= 9)
	{
		f.texture = true;
		f.normal = true;
		return (parse_face_push(dst, f));
	}
	else
	{
		fprintf(stderr, "f: illegal statement");
		return (false);
	}
}

#define N_KEYWORD (sizeof(g_kw_handler_lookup) / sizeof(*g_kw_handler_lookup))
struct s_kw_handler	g_kw_handler_lookup[] = {
	{"#", &comment_skip},
	{"v ", &parse_vertex},
	{"vn ", &parse_vertex_normal},
	{"vt ", &parse_vertex_texture},
	{"f ", &parse_face},

	{"p ", &unimplemented},
	{"l ", &unimplemented},

	{"vp ", &unimplemented},
	{"cstype ", &unimplemented},
	{"deg ", &unimplemented},
	{"bmat ", &unimplemented},
	{"step ", &unimplemented},
	{"curv ", &unimplemented},
	{"curv2 ", &unimplemented},
	{"surf ", &unimplemented},
	{"parm ", &unimplemented},
	{"trim ", &unimplemented},
	{"hole ", &unimplemented},
	{"scrv ", &unimplemented},
	{"sp ", &unimplemented},
	{"end ", &unimplemented},
	{"con ", &unimplemented},
	{"g ", &unimplemented},
	{"s ", &unimplemented},
	{"mg ", &unimplemented},
	{"o ", &unimplemented},
	{"bevel ", &unimplemented},
	{"c_interp ", &unimplemented},
	{"d_interp ", &unimplemented},
	{"lod ", &unimplemented},
	{"usemtl ", &unimplemented},
	{"mtllib ", &unimplemented},
	{"shadow_obj ", &unimplemented},
	{"trace_obj ", &unimplemented},
	{"ctech ", &unimplemented},
	{"stech ", &unimplemented},
};

static inline t_bool	is_prefix(char const *pre, char const *str)
{
	return (strncmp(pre, str, strlen(pre)) == 0);
}

static void	parse_line(t_wfobj *dst, char const *line, size_t linen)
{
	struct s_kw_handler	*match;
	size_t				i;

	i = 0;
	while (i < N_KEYWORD && !is_prefix(g_kw_handler_lookup[i].kw, line))
		i++;
	if (i == N_KEYWORD)
	{
		i = 0;
		while (*line && !isspace(line[i]))
			i++;
		fprintf(stderr, "unrecognized keyword `%.*s'", (int)i, line);
	}
	else
	{
		match = g_kw_handler_lookup + i;
		if (match->handle(dst, line + strlen(match->kw), match->kw))
			return ;
	}
	fprintf(stderr, ", skipping line %zu...\n", linen);
	return ;
}

t_wfobj	*wfobj_parse(FILE *stream)
{
	char	*line;
	size_t	size;
	size_t	linen;
	t_wfobj	*ret;

	ret = NULL;
	if ((ret = malloc(sizeof(*ret))))
	{
		bzero(ret, sizeof(*ret));
		line = NULL;
		size = 0;
		linen = 1;

		while (getline(&line, &size, stream) >= 0)
		{
			parse_line(ret, line, linen);
			linen++;
		}
		free(line);
	}
	else
		perror("error in wfobj_parse: malloc");
	return (ret);
}

void	wfobj_delete(t_wfobj *obj)
{
	t_list	*(lists[4]);
	t_list	*current;
	t_list	*tmp;
	size_t	i;

	lists[0] = obj->v;
	lists[1] = obj->vt;
	lists[2] = obj->vn;
	lists[3] = obj->f;
	i = 0;
	while (i < sizeof(lists) / sizeof(*lists))
	{
		current = lists[i];
		while (current != NULL)
		{
			free(current->content);
			tmp = current;
			current = current->next;
			free(tmp);
		}
		i++;
	}
	free(obj);
}
