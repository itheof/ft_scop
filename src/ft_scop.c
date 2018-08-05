/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_scop.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/18 09:12:21 by tvallee           #+#    #+#             */
/*   Updated: 2018/06/25 18:03:24 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_scop.h"
#include "libscop.h"
#include "cube.h"

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
};

t_cube	*g_cube = NULL;

void 	__attribute__ ((noreturn)) cleanup()
{
	libscop_deinit();
	/* This ^ will deinit all the gl ressources, programs, and our obects */
	glfwTerminate();
	exit(0);
}

int		main(int argc, char *argv[])
{
  double time;
  size_t	frames;

	if (!init(&g_env))
		return (-1);

	if (argc == 1)
		g_cube = objects_push(&g_cube_obj);
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
		if (glfwGetTime() > time + 1)
	 	{
			fprintf(stderr, "\rframes: %zu", frames);
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
