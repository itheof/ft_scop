/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_scop.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/21 13:17:01 by tvallee           #+#    #+#             */
/*   Updated: 2018/09/11 11:53:42 by tvallee          ###   ########.fr       */
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
# include "libscop.h"
# include "uniform.h"
# include "cube.h"

# include <GLFW/glfw3.h>

# define TEXTURES_DIR ASSETS_DIR "textures/"
# define SCROLL_SPEED 0.08
# define CAMERA_MOVE_SPEED 3
# define TEXTURING_SPEED 0.01
# define WIN_WIDTH 1280
# define WIN_HEIGHT 960

# define MAX_CUBES 256

typedef struct	s_env
{
	GLFWwindow	*window;
	t_camera	camera;
	t_bool		mouse_held;
	int			width;
	int			height;
}				t_env;

void	cube_toggle_texture(t_cube *obj);
t_bool	init(t_env *env);
void	register_callbacks(void);
void	update_camera(t_bool init);
void	push_cube(char const *model_path);
void	cube_focus_next(void);
void	cube_focus_prev(void);

/* functions that do not belong in the header, but 5 functions max */

void	key_callback(GLFWwindow *window,
		int key, int scancode, int action, int mods);

#endif
