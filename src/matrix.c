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
	off_t	off;

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

	off = (size_t)pos.x - ONE_INDEXED;
	if (pos.ndim >= 2)
		off += matrix->xlen * ((size_t)pos.y - ONE_INDEXED);
	if (pos.ndim >= 3)
		off += matrix->ylen * ((size_t)pos.z - ONE_INDEXED);
	if (pos.ndim >= 4)
		off += matrix->zlen * ((size_t)pos.w - ONE_INDEXED);
	printf("%f %f %f %f offset %zu\n", pos.x, pos.y, pos.z, pos.w, off);
	return (matrix->elems + off);
}
	
float		matrix_get(t_matrix *matrix, t_vector pos)
{
	return (*matrix_get_ptr(matrix, pos));
}

void		matrix_set(t_matrix *matrix, t_vector pos, float val)
{
	*matrix_get_ptr(matrix, pos) = val;
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

t_matrix	*matrix_new_id(size_t ndim)
{
	t_matrix	*ret;
	t_vector	size;
	size_t		i;

	if (ndim == 1)
		size = vec1(1);
	if (ndim == 2)
		size = vec2(2, 2);
	if (ndim == 3)
		size = vec3(3, 3, 3);
	if (ndim == 4)
		size = vec4(4, 4, 4, 4);
	if ((ret = matrix_new(size)))
	{
		i = 0;
		while (i < ndim)
		{
			matrix_set(ret, vec4(ONE_INDEXED + i, ONE_INDEXED + i,
						ONE_INDEXED + i, ONE_INDEXED + i), 1);
			i++;
		}
	}
	return (ret);
}
