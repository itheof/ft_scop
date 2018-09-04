/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_scop.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/18 09:12:21 by tvallee           #+#    #+#             */
/*   Updated: 2018/09/04 13:52:46 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_scop.h"
#include "libscop.h"
#include "cube.h"
#include "uniform.h"

t_env	g_env = {
	.camera = {
		.transform = {
			.translate = {
				.x = 0,
				.y = 0,
				.z = -2.0f,
				.ndim = 3,
			},
			.scale = {
				.ndim = 0,
			},
			.rotate = {
				.x = 0,
				.y = 0,
				.z = 0,
				.ndim = 3,
			},
			.rotangle = 0,
		},
	},
	.mouse_held = false,
	.width = WIN_WIDTH,
	.height = WIN_HEIGHT
};

int		g_cube_index = -1;
t_cube	*g_cubes[MAX_CUBES] = { NULL };

void 	__attribute__ ((noreturn)) cleanup()
{
	libscop_deinit();
	/* This ^ will deinit all the gl ressources, programs, and our obects */
	glfwTerminate();
	exit(0);
}

t_uniform_val	cube_is_current(void *obj)
{
	t_uniform_val	ret;

	ret.b = (obj == g_cubes[g_cube_index]);
	return (ret);
}

void	push_cube(void)
{
	if (g_cube_index + 1 >= MAX_CUBES)
	{
		fprintf(stderr, "already too many objects. skipping...\n");
		return ;
	}
	g_cube_index++;
	g_cubes[g_cube_index] = objects_push(&g_cube_obj);
}

void	cube_focus_next(void)
{
	if (g_cube_index == -1)
		return ;
	else if (g_cube_index == MAX_CUBES - 1)
		g_cube_index = 0;
	else if (g_cubes[g_cube_index + 1])
		g_cube_index++;
	else
		g_cube_index = 0;
}

void	cube_focus_prev(void)
{
	if (g_cube_index < 0)
		return ;
	else if (g_cube_index == 0)
	{
		while (g_cube_index + 1 < MAX_CUBES && g_cubes[g_cube_index + 1])
			g_cube_index++;
	}
	else
		g_cube_index--;
}

void	update_camera_translation(t_bool init)
{
	static double	x = 0.0f;
	static double	y = 0.0f;
	double			newx;
	double			newy;

	if (init)
		glfwGetCursorPos(g_env.window, &x, &y);
	else
	{
		glfwGetCursorPos(g_env.window, &newx, &newy);
		g_env.camera.transform.translate.x += (newx - x) / g_env.width * CAMERA_MOVE_SPEED;
		g_env.camera.transform.translate.y -= (newy - y) / g_env.height * CAMERA_MOVE_SPEED;
		/* the movement should be dependent on the camera z translate as well */
		x = newx;
		y = newy;
	}
}

int		main(int argc, char *argv[])
{
  double 	time;
  size_t	frames;
  char		title[256];

	if (!init(&g_env))
	{
		return (-1);
	}

	if (argc == 1)
		push_cube();
	else
	{
		(void)argv;
		;/*objects_push(for each argv);*/
	}
	time = glfwGetTime();
	frames = 0;
	fprintf(stderr, "frames: ");
    while (!glfwWindowShouldClose(g_env.window))
    {
		if (g_env.mouse_held)
			update_camera_translation(false);
		if (glfwGetTime() > time + 1)
	 	{
			snprintf ( title, sizeof(title),
                 "ft_scop - [FPS: %zu]", frames);
			glfwSetWindowTitle(g_env.window, title);
			frames = 0;
			time = glfwGetTime();
	 	}
	 	else
			frames++;
		objects_update();
		objects_render(g_env.camera);
		glfwSwapBuffers(g_env.window);
		glfwPollEvents();
    }
	cleanup();
}
