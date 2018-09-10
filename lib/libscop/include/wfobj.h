/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wfobj.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/07 11:07:38 by tvallee           #+#    #+#             */
/*   Updated: 2018/09/10 12:49:19 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WFOBJ_H
# define WFOBJ_H

# include "libft.h"

# define MAX_POLYGON_SIDES 4

# define COORD_X 0
# define COORD_Y 1
# define COORD_Z 2
# define COORD_W 3

# define TRIPLET_V 0
# define TRIPLET_VT 1
# define TRIPLET_VN 2

typedef struct	s_vertex
{
	float	coord[4];
	int		ndim;
}				t_vertex;

typedef struct	s_vertex_normal
{
	float	coord[3];
}				t_vertex_normal;

typedef struct	s_vertex_texture
{
	float	coord[3];
	int		ndim;
}				t_vertex_texture;

typedef struct	s_face
{
	int		triplets[MAX_POLYGON_SIDES][3];
	int		n_sides;
	t_bool	normal;
	t_bool	texture;
}				t_face;

typedef struct	s_wfobj
{
	t_list	*v;
	size_t	n_v;
	t_list	*vt;
	size_t	n_vt;
	t_list	*vn;
	size_t	n_vn;
	t_list	*f;
}				t_wfobj;

t_wfobj	*wfobj_parse(FILE *stream);
void	wfobj_delete(t_wfobj *obj);

#endif
