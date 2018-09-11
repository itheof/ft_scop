/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_scop.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/18 09:12:21 by tvallee           #+#    #+#             */
/*   Updated: 2018/09/11 16:33:42 by tvallee          ###   ########.fr       */
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
				.z = -4.5f,
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

void	cleanup(void)
{
	libscop_deinit();
	glfwTerminate();
	exit(0);
}

void	update_camera(t_bool init)
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
		if (glfwGetKey(g_env.window, GLFW_KEY_LEFT_CONTROL))
		{
			g_env.camera.transform.translate.x +=
				(newx - x) / g_env.width * CAMERA_MOVE_SPEED;
			g_env.camera.transform.translate.y -=
				(newy - y) / g_env.height * CAMERA_MOVE_SPEED;
		}
		x = newx;
		y = newy;
	}
}

static void	loop(void)
{
	double	time;
	double	old_time;
	size_t	frames;
	char	title[256];

	time = glfwGetTime();
	old_time = glfwGetTime();
	frames = 0;
	while (!glfwWindowShouldClose(g_env.window))
	{
		if (g_env.mouse_held)
			update_camera(false);
		if (old_time + 1 < time)
		{
			old_time = time;
			snprintf(title, sizeof(title), "ft_scop - [FPS: %zu]", frames);
			glfwSetWindowTitle(g_env.window, title);
			frames = 0;
		}
		frames++;
		objects_update(glfwGetTime() - time);
		time = glfwGetTime();
		objects_render(g_env.camera);
		glfwSwapBuffers(g_env.window);
		glfwPollEvents();
	}
}

int			main(int argc, char const *argv[])
{
	int	i;

	if (!init(&g_env))
		return (-1);
	if (argc != 1)
	{
		i = 1;
		while (i < argc)
		{
			push_cube(argv[i]);
			i++;
		}
	}
	loop();
	cleanup();
}
