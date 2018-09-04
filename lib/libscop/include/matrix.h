/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/04 14:17:07 by tvallee           #+#    #+#             */
/*   Updated: 2018/09/04 14:17:32 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATRIX_H
# define MATRIX_H

# include <stdlib.h>
# include "vector.h"

typedef struct	s_matrix
{
	size_t	xlen;
	size_t	ylen;
	size_t	nelem;
	float	elems[];
}				t_matrix;

typedef struct	s_transform
{
	t_vector	scale;
	t_vector	rotate;
	t_vector	translate;
	float		rotangle;
}				t_transform;

t_matrix		*matrix_new(t_vector size);
t_matrix		*matrix_new_id(size_t len);
t_matrix		*matrix_new_perspective(float fov, float ratio, float near,
		float far);
t_matrix		*matrix_new_from(t_vector vec);

void			matrix_id(t_matrix *m);
void			matrix_translate(t_matrix *m, t_vector trans);
void			matrix_scale(t_matrix *m, t_vector scale);
void			matrix_rotate(t_matrix *m, float angle, t_vector axis);

void			matrix_set(t_matrix *matrix, t_vector pos, float val);
void			matrix_dump(t_matrix const *matrix);
t_matrix		*matrix_mult(t_matrix const *left, t_matrix const *right);

void			matrix_deinit(t_matrix *m);
#endif
