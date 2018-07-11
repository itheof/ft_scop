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
				.z = -3.0f,
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

/*
t_texture g_tex_wall = {
	.path = TEXTURES_DIR "wall.ppm",
};

t_texture g_tex_face = {
	.path = TEXTURES_DIR "awesomeface.ppm",
};
*/

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
	if (!init(&g_env))
		return (-1);

	if (argc == 1)
		g_cube = objects_push(&g_cube_obj);
	else
	{
		(void)argv;
		;/*objects_push(for each argv);*/
	}
    while (!glfwWindowShouldClose(g_env.window))
    {
		objects_update();
		objects_render(g_env.camera);
		glfwSwapBuffers(g_env.window);
		glfwPollEvents();
    }
	cleanup();
}
