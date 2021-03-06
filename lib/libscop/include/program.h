/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   program.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/04 16:40:36 by tvallee           #+#    #+#             */
/*   Updated: 2018/09/10 17:32:36 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROGRAM_H
# define PROGRAM_H

# include "libft/stdbool.h"
# include "shader.h"
# include "matrix.h"
# include "uniform.h"

typedef struct	s_program
{
	t_shader		vertex;
	t_shader		fragment;
	unsigned		id;
	t_bool			gl_loaded;
	t_uniform const	(*uniforms)[];
}				t_program;

t_bool			program_init(t_program *p);
void			program_setb(t_program *p, char const *name, t_bool value);
void			program_seti(t_program *p, char const *name, int value);
void			program_set2i(t_program *p, char const *name, int val1,
		int val2);
void			program_setf(t_program *p, char const *name, float value);
void			program_set2f(t_program *p, char const *name, float val1,
		float val2);
void			program_set3f(t_program *p, char const *name, t_uniform_val val);
void			program_setmat4f(t_program *p, char const *name,
		t_matrix const *mat);
void			program_deinit(t_program *p);

#endif
