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

#include <math.h>
#include <stdlib.h>
#include "vector.h"

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

t_vector	normalize(t_vector v)
{
	float 		length;
	t_vector	res;

	if (!(length = sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z))))
		return (v);
	res.ndim = v.ndim;
	res.x = v.x / length;
	res.y = v.y / length;
	res.z = v.z / length;
	res.w = v.w / length;
	return (res);
}
