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

#include "ft_scop.h"
#include <assert.h>

#define ONE_INDEXED 1
#define DEBUG

static void	test_in_range(char letter, size_t x, size_t xmax)
{
	if (ONE_INDEXED > x)
	{
		dprintf(2, "matrix error: attempting to retrieve %c=%zu when "
				"zero indexing is enabled. Aborting\n", letter, x);
		abort();
	}
	if (x >= xmax + ONE_INDEXED)
	{
		dprintf(2, "matrix error: attempting to retrieve %c=%zu when "
				"%cmax=%zu. Aborting\n", letter, x, letter, xmax);
		abort();
	}
}

float		*matrix_get_ptr(t_matrix *matrix, t_vector pos)
{
	float	(*mat4)[matrix->wlen][matrix->zlen][matrix->ylen][matrix->xlen];
	float	(*mat3)[matrix->zlen][matrix->ylen][matrix->xlen];
	float	(*mat2)[matrix->ylen][matrix->xlen];
	float	(*mat1)[matrix->xlen];
#ifdef DEBUG
	if (pos.ndim != matrix->ndim)
	{
		dprintf(2, "matrix error: get_ptr was given two different dimensions "
				"(%zu and %zu)\n", pos.ndim, matrix->ndim);
		abort();
	}
	if (pos.ndim >= 1)
		test_in_range('x', pos.x, matrix->xlen);
	if (pos.ndim >= 2)
		test_in_range('y', pos.y, matrix->ylen);
	if (pos.ndim >= 3)
		test_in_range('z', pos.z, matrix->zlen);
	if (pos.ndim >= 4)
		test_in_range('w', pos.w, matrix->wlen);
#endif

	mat1 = (void*)&(matrix->elems);
	mat2 = (void*)&(matrix->elems);
	mat3 = (void*)&(matrix->elems);
	if (pos.ndim >= 4)
	{
		mat4 = (void*)&(matrix->elems);
		mat3 = &((*mat4)[(size_t)pos.w - ONE_INDEXED]);
	}
	if (pos.ndim >= 3)
		mat2 = &((*mat3)[(size_t)pos.z - ONE_INDEXED]);
	if (pos.ndim >= 2)
		mat1 = &((*mat2)[(size_t)pos.y - ONE_INDEXED]);
	return (*mat1 + (size_t)pos.x - ONE_INDEXED);
}

float		matrix_get(t_matrix *matrix, t_vector pos)
{
	return (*matrix_get_ptr(matrix, pos));
}

void		matrix_set(t_matrix *matrix, t_vector pos, float val)
{
	*matrix_get_ptr(matrix, pos) = val;
}

void		matrix_dump_2d(t_matrix *matrix)
{
	size_t	x;
	size_t	y;
	float	f;

	assert(matrix->ndim == 2);
	x = 0;
	while (x < matrix->xlen)
	{
		y = 0;
		while (y < matrix->ylen)
		{
			f = matrix_get(matrix, vec2(x + ONE_INDEXED, y + ONE_INDEXED));
			dprintf(2, " %f ", f);
			y++;
		}
		dprintf(2, "\n");
		x++;
	}
}

t_matrix	*matrix_new(t_vector size)
{
	t_matrix	*ret;
	size_t		nelem;

	nelem = (size_t)size.x;
	if (size.ndim >= 2)
		nelem *= (size_t)size.y;
	if (size.ndim >= 3)
		nelem *= (size_t)size.z;
	if (size.ndim >= 4)
		nelem *= (size_t)size.w;
	if ((ret = ft_memalloc(sizeof(t_matrix) + sizeof(float) * nelem)))
	{
		ret->xlen = (size_t)size.x;
		ret->ylen = (size_t)size.y;
		ret->zlen = (size_t)size.z;
		ret->wlen = (size_t)size.w;
		ret->nelem = nelem;
		ret->ndim = size.ndim;
	}
	return (ret);
}

t_matrix	*matrix_new_id(size_t len, size_t ndim)
{
	t_matrix	*ret;
	t_vector	size;
	size_t		i;

	assert(ndim < 5);
	if (ndim == 1)
		size = vec1(len);
	if (ndim == 2)
		size = vec2(len, len);
	if (ndim == 3)
		size = vec3(len, len, len);
	if (ndim == 4)
		size = vec4(len, len, len, len);
	if ((ret = matrix_new(size)))
	{
		i = 0;
		while (i < len)
		{
			if (ndim == 1)
				matrix_set(ret, vec1(ONE_INDEXED + i), 1);
			else if (ndim == 2)
				matrix_set(ret, vec2(ONE_INDEXED + i, ONE_INDEXED + i), 1);
			else if (ndim == 3)
				matrix_set(ret, vec3(ONE_INDEXED + i, ONE_INDEXED + i,
						ONE_INDEXED + i), 1);
			else
				matrix_set(ret, vec4(ONE_INDEXED + i, ONE_INDEXED + i,
						ONE_INDEXED + i, ONE_INDEXED + i), 1);
			i++;
		}
	}

	return (ret);
}
