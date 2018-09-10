/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uniform.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/04 16:40:00 by tvallee           #+#    #+#             */
/*   Updated: 2018/09/10 17:30:05 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UNIFORM_H
# define UNIFORM_H

enum			e_uniform_type
{
	E_UNIFORM_END,
	E_UNIFORM_BOOL,
	E_UNIFORM_INT,
	E_UNIFORM_2INT,
	E_UNIFORM_FLOAT,
	E_UNIFORM_2FLOAT,
	E_UNIFORM_3FLOAT,
	E_UNIFORM_MAT4FLOAT,
};

typedef union	u_uniform_val
{
	t_bool			b;
	int				i;
	int				i2[2];
	float			f;
	float			f2[2];
	float			f3[3];
	t_matrix const	*mat;
}				t_uniform_val;

typedef struct	s_uniform
{
	enum e_uniform_type	type;
	char const			*name;
	t_uniform_val		(*resolve)(void *obj);
}				t_uniform;

#endif
