/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wfobj.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/07 10:41:37 by tvallee           #+#    #+#             */
/*   Updated: 2018/09/07 12:33:44 by tvallee          ###   ########.fr       */
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
	fprintf(stderr, "keyword `%s'unimplemented yet", kw);
	return (false);
}

/*
static t_bool	parse_line(char const *line, t_list ** vertices_h, t_list **faces_h)
{
	float	vbuf[3];
	int		fbuf[5];
	t_list	*lst;

	if (!line)
	{
		*vertices_h = NULL;
		*faces_h = NULL;
		return (true);
	}

	if (!strncmp(line, "v ", 2))
	{
		if (sscanf(line + 2, "%f %f %f", vbuf, vbuf + 1, vbuf + 2) != 3)
			fprintf(stderr, "unsufficient vertex input\n");
		else
		{
			if (!(lst = ft_lstnew(vbuf, sizeof(vbuf))))
			{
				perror("parse line malloc: ");
				return (false);
			}
			ft_lstpushback(vertices_h, lst);
		}
	}

	else if (!strncmp(line, "f ", 2))
	{
		if ((fbuf[0] = sscanf(line + 2, "%d %d %d %d", fbuf + 1, fbuf + 2, fbuf + 3, fbuf + 4)) < 3)
			fprintf(stderr, "unsufficient face input\n");
		else
		{
			if (!(lst = ft_lstnew(fbuf, sizeof(fbuf))))
			{
				perror("parse line malloc: ");
				return (false);
			}
			ft_lstpushback(faces_h, lst);
		}
	}

	else if (line[0] == '#')
		;
	else
		fprintf(stderr, "warning: ignored line: %s\n", line);
	return (true);
}
*/

#define N_KEYWORD (sizeof(g_kw_handler_lookup) / sizeof(*g_kw_handler_lookup))
struct s_kw_handler	g_kw_handler_lookup[] = {
	{"#", &comment_skip},
	{"v ", &unimplemented},
	{"f ", &unimplemented}
};

static inline t_bool	is_prefix(char const *pre, char const *str)
{
	return (strncmp(pre, str, strlen(pre)) == 0);
}

static t_bool	parse_line(t_wfobj *dst, char const *line, size_t linen)
{
	size_t	i;

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
	else if (g_kw_handler_lookup[i].handle(dst, line, g_kw_handler_lookup[i].kw))
		return (true);
	fprintf(stderr, ", ignoring line %zu...\n", linen);
	return (true);
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
			if (!parse_line(ret, line, linen))
				break;
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
	(void)obj;
}
