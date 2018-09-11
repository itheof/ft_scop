/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   callbacks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/11 11:45:18 by tvallee           #+#    #+#             */
/*   Updated: 2018/09/11 16:36:10 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "ft_scop.h"
#include "cube.h"

extern t_env	g_env;

static void		scroll_callback(GLFWwindow *window,
		double xoffset, double yoffset)
{
	(void)window;
	(void)xoffset;
	g_env.camera.transform.translate.z +=
		g_env.camera.transform.translate.z * yoffset * SCROLL_SPEED;
}

static void		mouse_button_callback(GLFWwindow *window,
		int button, int action, int mods)
{
	(void)window;
	(void)button;
	(void)mods;
	g_env.mouse_held = (action == GLFW_PRESS);
	update_camera(true);
}

void			framebuffer_callback(GLFWwindow *window, int width, int height)
{
	(void)window;
	display_set_viewport(width, height);
	g_env.width = width;
	g_env.height = height;
}

void			window_refresh_callback(GLFWwindow *window)
{
	objects_render(g_env.camera);
	glfwSwapBuffers(window);
}

void			drop_callback(GLFWwindow *window, int nfiles, char const **paths)
{
	char const	*ext;
	int			i;

	(void)window;
	i = 0;
	while (i < nfiles)
	{
		ext = ft_strrchr(paths[i], '.');
		if (ext)
		{
			ext++;
			if (!strcmp(ext, "obj"))
				push_cube(paths[i]);
			else if (!strcmp(ext, "ppm"))
				fprintf(stderr, "ppm hot loading not implemented yet\n");
			else
				fprintf(stderr, "unsupported file extension: %s. Skipping\n", ext);
		}
		else
			fprintf(stderr, "unsupported file: %s. Skipping\n", paths[i]);
		i++;
	}
}

void			register_callbacks(void)
{
	glfwSetKeyCallback(g_env.window, key_callback);
	glfwSetScrollCallback(g_env.window, scroll_callback);
	glfwSetMouseButtonCallback(g_env.window, mouse_button_callback);
	glfwSetFramebufferSizeCallback(g_env.window, framebuffer_callback);
	glfwSetWindowRefreshCallback(g_env.window, window_refresh_callback);
	glfwSetDropCallback(g_env.window, drop_callback);
}
