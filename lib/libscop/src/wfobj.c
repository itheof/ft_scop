/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wfobj.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/07 10:41:37 by tvallee           #+#    #+#             */
/*   Updated: 2018/09/11 11:08:18 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
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

static void		update_extreme_vertex(t_wfobj *dst, t_vertex vertex)
{
	int	i;

	i = 0;
	if (dst->extreme_unset)
	{
		while (i < vertex.ndim)
		{
			dst->min_vertex.coord[i] = vertex.coord[i];
			dst->max_vertex.coord[i] = vertex.coord[i];
			i++;
		}
		dst->extreme_unset = false;
	}
	else
	{
		while (i < vertex.ndim)
		{
			if (dst->min_vertex.coord[i] > vertex.coord[i])
				dst->min_vertex.coord[i] = vertex.coord[i];
			if (dst->max_vertex.coord[i] < vertex.coord[i])
				dst->max_vertex.coord[i] = vertex.coord[i];
			i++;
		}
	}
}

static t_bool	parse_vertex(t_wfobj *dst, char const *line, char const *kw)
{
	t_vertex	vertex;
	t_list		*lst;

	(void)kw;
	vertex.ndim = sscanf(line, "%f %f %f %f", vertex.coord + COORD_X,
			vertex.coord + COORD_Y, vertex.coord + COORD_Z, vertex.coord + COORD_W);
	if (vertex.ndim < 3)
		fprintf(stderr, "insufficient vertex input");
	else
	{
		if (vertex.ndim != 4)
			vertex.coord[COORD_W] = 1.0f;
		if (!(lst = ft_lstnew(&vertex, sizeof(vertex))))
		{
			perror("parse_vertex malloc");
			return (false);
		}
		if (dst->v_tail)
		{
			dst->v_tail->next = lst;
			dst->v_tail = lst;
		}
		else
		{
			dst->v = lst;
			dst->v_tail = lst;
		}
		dst->n_v++;
	}
	update_extreme_vertex(dst, vertex);
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
		fprintf(stderr, "insufficient vertex normal input");
		return (false);
	}
	if (!(lst = ft_lstnew(&vn, sizeof(vn))))
	{
		perror("parse_vertex_normal malloc");
		return (false);
	}
	if (dst->vn_tail)
	{
		dst->vn_tail->next = lst;
		dst->vn_tail = lst;
	}
	else
	{
		dst->vn = lst;
		dst->vn_tail = lst;
	}
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
		fprintf(stderr, "insufficient vertex texture input");
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
	if (dst->vt_tail)
	{
		dst->vt_tail->next = lst;
		dst->vt_tail = lst;
	}
	else
	{
		dst->vt = lst;
		dst->vt_tail = lst;
	}
	dst->n_vt++;
	return (true);
}

static t_bool	normalize_index(int *indexp, size_t imax)
{
	if (imax > INT_MAX) //XXX
		return (false);
	if (*indexp > (int)imax)
		return (false);
	else if (*indexp < 0)
	{
		if (*indexp == INT_MIN) //XXX
			return (false);
		if (-1 * (*indexp) > (int)imax)
			return (false);
		*indexp *= -1;
	}
	return (true);
}

static t_bool	normalize_face(t_wfobj *obj, t_face *f)
{
	int	i;

	i = 0;
	while (i < f->n_sides)
	{
		if (!normalize_index(&f->triplets[i][TRIPLET_V], obj->n_v))
			return (false);
		if (f->texture && !normalize_index(&f->triplets[i][TRIPLET_VT], obj->n_vt))
			return (false);
		if (f->normal && !normalize_index(&f->triplets[i][TRIPLET_VN], obj->n_vn))
			return (false);
		i++;
	}
	return (true);
}

static t_bool	parse_face_push(t_wfobj *dst, t_face *f)
{
	t_list	*lst;

	if (!normalize_face(dst, f))
	{
		fprintf(stderr, "invalid face coordinates");
		return (false);
	}
	if (!(lst = ft_lstnew(f, sizeof(*f))))
	{
		fprintf(stderr, "parse_face_push malloc");
		return (false);
	}
	if (dst->f_tail)
	{
		dst->f_tail->next = lst;
		dst->f_tail = lst;
	}
	else
	{
		dst->f = lst;
		dst->f_tail = lst;
	}
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
		return (parse_face_push(dst, &f));
	}
	else if ((f.n_sides = sscanf(line, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d/",
				&f.triplets[0][TRIPLET_V], &f.triplets[0][TRIPLET_VT], &f.triplets[0][TRIPLET_VN],
				&f.triplets[1][TRIPLET_V], &f.triplets[1][TRIPLET_VT], &f.triplets[1][TRIPLET_VN],
				&f.triplets[2][TRIPLET_V], &f.triplets[2][TRIPLET_VT], &f.triplets[2][TRIPLET_VN],
				&f.triplets[3][TRIPLET_V], &f.triplets[3][TRIPLET_VT], &f.triplets[3][TRIPLET_VN]
				) / 3) >= 3)
	{
		f.texture = true;
		f.normal = true;
		return (parse_face_push(dst, &f));
	}
	else if ((f.n_sides = sscanf(line, "%d// %d// %d// %d//",
				&f.triplets[0][TRIPLET_V], &f.triplets[1][TRIPLET_V],
				&f.triplets[2][TRIPLET_V], &f.triplets[3][TRIPLET_V])) >= 3)
	{
		f.normal = false;
		f.texture = false;
		return (parse_face_push(dst, &f));
	}
	else if ((f.n_sides = sscanf(line, "%d/%d/ %d/%d/ %d/%d/ %d/%d/",
				&f.triplets[0][TRIPLET_V], &f.triplets[0][TRIPLET_VT],
				&f.triplets[1][TRIPLET_V], &f.triplets[1][TRIPLET_VT],
				&f.triplets[2][TRIPLET_V], &f.triplets[2][TRIPLET_VT],
				&f.triplets[3][TRIPLET_V], &f.triplets[3][TRIPLET_VT]
				) / 2) >= 3)
	{
		f.texture = true;
		f.normal = false;
		return (parse_face_push(dst, &f));
	}
	else if ((f.n_sides = sscanf(line, "%d//%d %d//%d %d//%d %d//%d",
				&f.triplets[0][TRIPLET_V], &f.triplets[0][TRIPLET_VN],
				&f.triplets[1][TRIPLET_V], &f.triplets[1][TRIPLET_VN],
				&f.triplets[2][TRIPLET_V], &f.triplets[2][TRIPLET_VN],
				&f.triplets[3][TRIPLET_V], &f.triplets[3][TRIPLET_VN]
				) / 2) >= 3)
	{
		f.texture = false;
		f.normal = true;
		return (parse_face_push(dst, &f));
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
		ret->extreme_unset = true;
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
