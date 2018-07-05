/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_scop.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/21 13:17:01 by tvallee           #+#    #+#             */
/*   Updated: 2018/06/25 18:03:15 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SCOP_H
# define FT_SCOP_H

# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "libft/stdbool.h"
# include "libft/libc.h"
# include "libft/print.h"
# include "vector.h"
# include "matrix.h"
# include "file.h"
# include "program.h"
# include "angle_conv.h"
# include "texture.h"
# include <glad/glad.h>
# include <GLFW/glfw3.h>

# define TEXTURES_DIR ASSETS_DIR "textures/"
# define SCROLL_SPEED 0.08

typedef struct	s_env
{
	GLFWwindow	*window;
	struct		s_framebuffer
	{
		int		width;
		int		height;
	}			buf;
	t_program	current_glprogram;
	t_bool			wireframe;
	t_vector	translate;
	t_vector	scale;
	t_vector	rotate;
	float		rotangle;
	t_matrix	*model;
	struct		s_camera
	{
		t_vector	translate;
		t_vector	scale;
		t_vector	rotate;
		float		rotangle;
	}			camera;
}				t_env;

t_bool	init(t_env *env);
void	register_callbacks(void);
void	render(void);

#endif
