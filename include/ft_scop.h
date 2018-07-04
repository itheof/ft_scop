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

# define _POSIX_C_SOURCE 200809L
# include <math.h>
# include <ctype.h>
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
# include <glad/glad.h>
# include <GLFW/glfw3.h>

# ifndef M_PI
#  define M_PI 3.14159265358979323846264338327950288
# endif
# define ASSETS_DIR "assets/"
# define TEXTURES_DIR ASSETS_DIR "textures/"
# define SHADERS_DIR ASSETS_DIR "shaders/"
# define SCROLL_SPEED 0.08

typedef	struct	s_shader
{
	unsigned 	id;
	GLenum		type;
	char const	*path;
}				t_shader;

typedef struct	s_texture
{
	unsigned		id;
	char const		*path;
	unsigned char	*data;
	int				width;
	int				height;
}				t_texture;

typedef struct	s_env
{
	GLFWwindow	*window;
	struct		s_framebuffer
	{
		int		width;
		int		height;
	}			buf;
	unsigned int	current_glprogram;
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

t_bool	program_init(unsigned *id);
void 	program_setb(unsigned int id, char const *name, t_bool value);
void 	program_seti(unsigned int id, char const *name, int value);
void 	program_set2i(unsigned int id, char const *name, int val1, int val2);
void	program_setf(unsigned int id, char const *name, float value);
void	program_set2f(unsigned int id, char const *name, float val1, float val2);
void	program_setmat4f(unsigned int id, char const *name, t_matrix const *mat);
void	program_deinit(unsigned id);

t_bool		shader_init(t_shader *shader);
void		shader_deinit(t_shader *shader);

t_bool		texture_init(t_texture *t);
void		texture_deinit(t_texture *t);

unsigned char	*ppm_load(char const *path, int *width, int *height);

float	ft_degree(float rad);
float	ft_radian(float degree);
#endif
