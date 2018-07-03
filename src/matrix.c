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

#define DEBUG

static void	test_in_range(char letter, size_t x, size_t xmax)
{
	if (x >= xmax)
	{
		dprintf(2, "matrix error: attempting to retrieve %c=%zu when "
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
		dprintf(2, "matrix error: get_ptr was given a bad vector dimension "
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
		dprintf(2, "matrix error: malloc failed\n");
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
		dprintf(2, "matrix error: malloc failed\n");
		abort();
	}
	matrix_deinit(scalemat);
}
