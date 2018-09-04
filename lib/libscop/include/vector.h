/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/04 16:39:22 by tvallee           #+#    #+#             */
/*   Updated: 2018/09/04 16:40:19 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_H
# define VECTOR_H

# include <stdlib.h>

typedef struct	s_vector
{
	size_t		ndim;
	float		x;
	float		y;
	float		z;
	float		w;
}				t_vector;

t_vector		vec1(float x);
t_vector		vec2(float x, float y);
t_vector		vec3(float x, float y, float z);
t_vector		vec4(float x, float y, float z, float w);
t_vector		normalize(t_vector v);

#endif
