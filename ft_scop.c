/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_scop.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/18 09:12:21 by tvallee           #+#    #+#             */
/*   Updated: 2018/06/18 09:12:44 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

#include "libft/stdbool.h"
#include "libft/print.h"
#include "GLFW/glfw3.h"

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"

"void main()\n"
"{\n"
"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\n\0";

const char	*fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"

"void main()\n"
"{\n"
"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

typedef struct	s_env
{
	GLFWwindow	*window;
	struct		s_framebuffer
	{
		int		width;
		int		height;
	}			buf;
	unsigned int	current_glprogram;
}				t_env;

t_env	g_env;

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

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if ((key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) ||
			(key == GLFW_KEY_C && action == GLFW_PRESS && mods & GLFW_MOD_CONTROL))
        glfwSetWindowShouldClose(window, GLFW_TRUE);
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

t_bool	init_shaders(void)
{
	int  success;
	char infoLog[512];
	static unsigned int	vertexShader = 0;
	static unsigned int fragmentShader = 0;
	static unsigned int shaderProgram = 0;

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
		dprintf(2, "ERROR: shader vertex compilation failed: %s\n", infoLog);
		return (false);
	}
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, sizeof(infoLog), NULL, infoLog);
		dprintf(2, "ERROR: shader fragment compilation failed: %s\n", infoLog);
		return (false);
	}

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		dprintf(2, "ERROR: shader program compilation failed: %s\n", infoLog);
		return (false);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	g_env.current_glprogram = shaderProgram;
	return (true);
}

t_bool	init(void)
{
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
        return (false);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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
	glfwSetFramebufferSizeCallback(g_env.window, framebuffer_callback);
	glfwGetFramebufferSize(g_env.window, &g_env.buf.width, &g_env.buf.height);
	glViewport(0, 0, g_env.buf.width, g_env.buf.height);
	glfwSwapInterval(1);
	return (true);
}

void	render(void)
{
	static float const vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
	};
	static int	first_run = 1;
	static unsigned int	VBO;
	static unsigned int	VAO;

	if (first_run)
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glUseProgram(g_env.current_glprogram);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		first_run = 0;
	}
	glUseProgram(g_env.current_glprogram);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

int		main(void)
{
	if (!init())
		return (-1);

    while (!glfwWindowShouldClose(g_env.window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

		render();

        glfwSwapBuffers(g_env.window);
        glfwPollEvents();
    }
	cleanup();
}
