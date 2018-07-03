/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/25 15:38:03 by tvallee           #+#    #+#             */
/*   Updated: 2018/06/25 16:02:09 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_scop.h"

t_vector	vec1(float x)
{
	t_vector	vect;

	vect.ndim = 1;
	vect.x = x;
	return (vect);
}

t_vector	vec2(float x, float y)
{
	t_vector	vect;

	vect.ndim = 2;
	vect.x = x;
	vect.y = y;
	return (vect);
}

t_vector	vec3(float x, float y, float z)
{
	t_vector	vect;

	vect.ndim = 3;
	vect.x = x;
	vect.y = y;
	vect.z = z;
	return (vect);
}

t_vector	vec4(float x, float y, float z, float w)
{
	t_vector	vect;

	vect.ndim = 4;
	vect.x = x;
	vect.y = y;
	vect.z = z;
	vect.w = w;
	return (vect);
}

t_matrix	*vec_to_matrix(t_vector vec)
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
