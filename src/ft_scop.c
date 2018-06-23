/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_scop.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/18 09:12:21 by tvallee           #+#    #+#             */
/*   Updated: 2018/06/23 17:54:21 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "libft/stdbool.h"
#include "libft/print.h"
#include "ft_scop.h"
#include "GLFW/glfw3.h"

#define ASSETS_DIR "assets/"

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
	struct		s_offset
	{
		float	x;
		float	y;
	}			off;
}				t_env;

t_env	g_env;

static t_texture g_tex_wall = {
	.path = ASSETS_DIR "wall.ppm",
};
static t_texture g_tex_face = {
	.path = ASSETS_DIR "awesomeface.ppm",
};

void	render(void);

void 	__attribute__ ((noreturn)) cleanup()
{
	glfwTerminate();
	exit(0);
}

void error_callback(int error, const char* description)
{
	(void)error;
    ft_putstr_fd("GLFW error: ", 2);
	ft_putendl_fd(description, 2);
}

static void	toggle_wireframe_mode(void)
{
	g_env.wireframe = !g_env.wireframe;
	if (g_env.wireframe)
	{
		ft_putendl_fd("wireframe on", 2);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		ft_putendl_fd("wireframe off", 2);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if ((key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) ||
			(key == GLFW_KEY_Q && action == GLFW_PRESS))
        glfwSetWindowShouldClose(window, GLFW_TRUE);
	else if (key == GLFW_KEY_W && action == GLFW_PRESS)
		toggle_wireframe_mode();
	else if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		g_env.off.y = 0;
		g_env.off.x = 0;
	}
	else if (key == GLFW_KEY_UP)
		g_env.off.y += 0.05;
	else if (key == GLFW_KEY_DOWN)
		g_env.off.y -= 0.05;
	else if (key == GLFW_KEY_LEFT)
		g_env.off.x -= 0.05;
	else if (key == GLFW_KEY_RIGHT)
		g_env.off.x += 0.05;
	else
		printf("key: %d scancode %d action %d mods %d\n", key, scancode, action, mods);
}

void	framebuffer_callback(GLFWwindow *window, int width, int height)
{
	(void)window;
	g_env.buf.width = width;
	g_env.buf.height = height;
	glViewport(0, 0, width, height);
}

void 	window_refresh_callback(GLFWwindow* window)
{
    render();
    glfwSwapBuffers(window);
}

t_bool	init_shaders(void)
{
	if (!program_init(&g_env.current_glprogram))
		return (false);
	return (true);
}

t_bool	init(void)
{
	g_env.off.x = 0;
	g_env.off.y = 0;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
        return (false);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    g_env.window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!g_env.window)
    {
		ft_putstr_fd("Could not open window", 2);
        glfwTerminate();
        return (false);
    }
	glfwSetKeyCallback(g_env.window, key_callback);
    glfwMakeContextCurrent(g_env.window);
	if (!init_shaders())
	{
		ft_putendl_fd("Failed to compile shaders", 2);
		glfwTerminate();
		return (false);
	}
	if (!texture_init(&g_tex_wall) || !texture_init(&g_tex_face))
	{
		ft_putendl_fd("Failed to load textures", 2);
	}
	glfwSetFramebufferSizeCallback(g_env.window, framebuffer_callback);
	glfwGetFramebufferSize(g_env.window, &g_env.buf.width, &g_env.buf.height);
	glfwSetWindowRefreshCallback(g_env.window, window_refresh_callback);
	glViewport(0, 0, g_env.buf.width, g_env.buf.height);
	glfwSwapInterval(1);
	return (true);
}

void	render(void)
{
	static float const vertices[] = {
		 // positions        // colors          // texture coords
		 0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, // top right
		 0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f, // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f, // bottom left
		-0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f // bottom up
		//NOTE: Top and bottom coords are inverted so we dont have to reverse the image
	};
	static unsigned int const	indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	static int	first_run = 1;

	static unsigned int	VAO;
	static unsigned int	VBO;
	static unsigned int	EBO;

	if (first_run)
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		first_run = 0;
		glUseProgram(g_env.current_glprogram);
		program_seti(g_env.current_glprogram, "texture1", 0);
		program_seti(g_env.current_glprogram, "texture2", 1);
	}
	glClear(GL_COLOR_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_tex_wall.id);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, g_tex_face.id);
	program_set2f(g_env.current_glprogram, "off", g_env.off.x, g_env.off.y);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
/*	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);*/
}

int		main(void)
{

	if (!init())
		return (-1);

    while (!glfwWindowShouldClose(g_env.window))
    {
		render();
        glfwSwapBuffers(g_env.window);
        glfwPollEvents();
    }
	cleanup();
}
