/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/25 15:18:09 by tvallee           #+#    #+#             */
/*   Updated: 2018/06/25 18:12:56 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _POSIX_C_SOURCE 200809L

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "libft/libc.h"
#include "vector.h"
#include "matrix.h"

#define DEBUG

static void	test_in_range(char letter, size_t x, size_t xmax)
{
	if (x >= xmax)
	{
		fprintf(stderr, "matrix error: attempting to retrieve %c=%zu when "
				"%cmax=%zu. Aborting\n", letter, x, letter, xmax);
		abort();
	}
}

float		*matrix_get_ptr(t_matrix const *matrix, t_vector pos)
{
	float	(*mat)[matrix->xlen][matrix->ylen];

#ifdef DEBUG
	if (pos.ndim != 2)
	{
		fprintf(stderr, "matrix error: get_ptr was given a bad vector dimension "
				"(%zu, expected %d)\n", pos.ndim, 2);
		abort();
	}
	test_in_range('x', pos.x, matrix->xlen);
	test_in_range('y', pos.y, matrix->ylen);
#endif

	mat = (void*)&(matrix->elems);
	return (&(*mat)[(size_t)pos.x][(size_t)pos.y]);
}

float		matrix_get(t_matrix const *matrix, t_vector pos)
{
	return (*matrix_get_ptr(matrix, pos));
}

void		matrix_set(t_matrix *matrix, t_vector pos, float val)
{
	*matrix_get_ptr(matrix, pos) = val;
}

void		matrix_dump(t_matrix const *matrix)
{
	size_t	x;
	size_t	y;
	float	f;

	x = 0;
	while (x < matrix->xlen)
	{
		y = 0;
		while (y < matrix->ylen)
		{
			f = matrix_get(matrix, vec2(x, y));
			fprintf(stderr, " %f ", f);
			y++;
		}
		fprintf(stderr, "\n");
		x++;
	}
}

t_matrix	*matrix_new(t_vector size)
{
	t_matrix	*ret;
	size_t		nelem;

	nelem = (size_t)size.x * (size_t)size.y;
	if ((ret = ft_memalloc(sizeof(t_matrix) + sizeof(float) * nelem)))
	{
		ret->xlen = (size_t)size.x;
		ret->ylen = (size_t)size.y;
		ret->nelem = nelem;
	}
	return (ret);
}

void		matrix_id(t_matrix *m)
{
	size_t	i;

	assert(m->xlen == m->ylen);
	i = 0;
	ft_bzero(m->elems, sizeof(*m->elems) * m->nelem);
	while (i < m->xlen)
	{
		matrix_set(m, vec2(i, i), 1);
		i++;
	}
}

t_matrix	*matrix_new_id(size_t len)
{
	t_matrix	*ret;

	if ((ret = matrix_new(vec2(len, len))))
		matrix_id(ret);
	return (ret);
}

static float	matrix_multsum(t_matrix const *left, t_matrix const *right, size_t x, size_t y)
{
	size_t	c;
	float	count;

	c = 0;
	count = 0;
	while (c < left->xlen)
	{
		count += matrix_get(left, vec2(x, c)) * matrix_get(right, vec2(c, y));
		c++;
	}
	return (count);
}

t_matrix	*matrix_mult(t_matrix const *left, t_matrix const* right)
{
	t_matrix	*ret;
	size_t		x;
	size_t		y;

	assert(left->ylen == right->xlen);
	if ((ret = matrix_new(vec2(left->xlen, right->ylen))))
	{
		x = 0;
		while (x < ret->xlen)
		{
			y = 0;
			while (y < ret->ylen)
			{
				matrix_set(ret, vec2(x, y), matrix_multsum(left, right, x, y));
				y++;
			}
			x++;
		}
	}
	return (ret);
}

void		matrix_deinit(t_matrix *m)
{
	free(m);
}

void	matrix_translate(t_matrix *m, t_vector trans)
{
	t_matrix	*transmat;
	t_matrix	*tmp;
	size_t		n;
	float		*p;

	if (!(transmat = matrix_new_id(m->ylen)))
		return ;
	n = 0;
	p = &(trans.x);
	while (n < trans.ndim)
	{
		matrix_set(transmat, vec2(n, 3), p[n]);
		n++;
	}
	if ((tmp = matrix_mult(m, transmat)))
	{
		ft_memcpy(m, tmp, sizeof(*m) + sizeof(float) * tmp->nelem);
		matrix_deinit(tmp);
	}
	else
	{
		fprintf(stderr, "matrix error: malloc failed\n");
		abort();
	}
	matrix_deinit(transmat);
}

void	matrix_scale(t_matrix *m, t_vector scale)
{
	t_matrix	*scalemat;
	t_matrix	*tmp;
	size_t		n;
	float		*p;

	if (!(scalemat = matrix_new_id(m->ylen)))
		return ;
	n = 0;
	p = &(scale.x);
	while (n < scale.ndim)
	{
		matrix_set(scalemat, vec2(n, n), p[n]);
		n++;
	}
	if ((tmp = matrix_mult(m, scalemat)))
	{
		ft_memcpy(m, tmp, sizeof(*m) + sizeof(float) * tmp->nelem);
		matrix_deinit(tmp);
	}
	else
	{
		fprintf(stderr, "matrix error: malloc failed\n");
		abort();
	}
	matrix_deinit(scalemat);
}

void	matrix_rotate(t_matrix *m, float angle, t_vector axis)
{
	t_matrix	*rotmat;
	t_matrix	*tmp;
	t_vector	temp;
	float		c;
	float		s;

	assert(m->ylen == 4 && m->xlen == 4);
	if (!angle || !(rotmat = matrix_new_id(m->ylen)))
		return ;
	c = cosf(angle);
	s = sinf(angle);
	axis = normalize(axis);
	temp.ndim = axis.ndim;
	temp.x = ((1.0f - c) * axis.x);
	temp.y = ((1.0f - c) * axis.y);
	temp.z = ((1.0f - c) * axis.z);
	matrix_set(rotmat, vec2(0, 0), c + axis.x * temp.x);
	matrix_set(rotmat, vec2(1, 0), temp.x * axis.y + s * axis.z);
	matrix_set(rotmat, vec2(2, 0), temp.x * axis.z - s * axis.y);
	matrix_set(rotmat, vec2(0, 1), temp.y * axis.x - s * axis.z);
	matrix_set(rotmat, vec2(1, 1), c + temp.y * axis.y);
	matrix_set(rotmat, vec2(2, 1), temp.y * axis.z + s * axis.x);
	matrix_set(rotmat, vec2(0, 2), temp.z * axis.x + s * axis.y);
	matrix_set(rotmat, vec2(1, 2), temp.z * axis.y - s * axis.x);
	matrix_set(rotmat, vec2(2, 2), c + temp.z * axis.z);
	if ((tmp = matrix_mult(m, rotmat)))
	{
		ft_memcpy(m, tmp, sizeof(*m) + sizeof(float) * tmp->nelem);
		matrix_deinit(tmp);
	}
	else
	{
		fprintf(stderr, "matrix error: malloc failed\n");
		abort();
	}
	matrix_deinit(rotmat);
}

t_matrix	*matrix_new_perspective(float fov, float ratio, float near, float far)
{
	t_matrix	*ret;
	float 		tanhalf;

	if (!(ret = matrix_new(vec2(4, 4))))
	{
		fprintf(stderr, "matrix error: malloc failed\n");
		abort();
	}
	tanhalf = tanf(fov / 2.0f);
	ft_bzero(ret->elems, sizeof(*ret->elems) * ret->nelem);
	matrix_set(ret, vec2(0, 0), 1.0f / (ratio * tanhalf));
	matrix_set(ret, vec2(1, 1), 1.0f / (tanhalf));
	matrix_set(ret, vec2(2, 2), - (far + near ) / (far - near));
	matrix_set(ret, vec2(2, 3), - ((2.0f * far * near) / (far - near)));
	matrix_set(ret, vec2(3, 2), -1.0f);
	return (ret);
}

t_matrix	*matrix_new_from(t_vector vec)
{
	t_matrix	*ret;
	float		*p;
	size_t		n;

	if ((ret = matrix_new(vec2(vec.ndim, 1))))
	{
		n = 0;
		p = &(vec.x);
		while (n < vec.ndim)
		{
			matrix_set(ret, vec2(n, 0), p[n]);
			n++;
		}
	}
	return(ret);
}
