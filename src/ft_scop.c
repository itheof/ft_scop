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

static t_env	g_env = {
	.translate = {
		.x = 0,
		.y = 0,
		.ndim = 2,
	},
	.scale = {
		.x = 1,
		.y = 1,
		.ndim = 2,
	},
	.rotate = {
		.x = 0,
		.y = 0,
		.ndim = 2,
	},
	.rotangle = 0,
	.camera = {
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
	}
};

static t_texture g_tex_wall = {
	.path = ASSETS_DIR "wall.ppm",
};

static t_texture g_tex_face = {
	.path = ASSETS_DIR "awesomeface.ppm",
};

void	render(void);

void 	__attribute__ ((noreturn)) cleanup()
{
	program_deinit(g_env.current_glprogram);
	texture_deinit(&g_tex_wall);
	texture_deinit(&g_tex_face);
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

static void	scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	float	new_scale;

	(void)window;
	(void)xoffset;
	g_env.camera.translate.z += g_env.camera.translate.z * yoffset * SCROLL_SPEED;
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
		g_env.translate.y = 0;
		g_env.translate.x = 0;
	}
	else if (key == GLFW_KEY_H)
		g_env.camera.translate.x -= 0.05;
	else if (key == GLFW_KEY_J)
		g_env.camera.translate.y -= 0.05;
	else if (key == GLFW_KEY_K)
		g_env.camera.translate.y += 0.05;
	else if (key == GLFW_KEY_L)
		g_env.camera.translate.x += 0.05;
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
	glfwSetScrollCallback(g_env.window, scroll_callback);
    glfwMakeContextCurrent(g_env.window);
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
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
	if (!(g_env.model = matrix_new_id(4)))
	{
		/*do the twist*/
		;
	}
	return (true);
}

void	render(void)
{
	static float const vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

	static int	first_run = 1;

	static unsigned int	VAO;
	static unsigned int	VBO;

	if (first_run)
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glEnable(GL_DEPTH_TEST);

		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		first_run = 0;
		glUseProgram(g_env.current_glprogram);
		program_seti(g_env.current_glprogram, "texture1", 0);
		program_seti(g_env.current_glprogram, "texture2", 1);
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	matrix_id(g_env.model);
	matrix_scale(g_env.model, g_env.scale);
	matrix_rotate(g_env.model, (float)glfwGetTime(), vec3(0.5f, 1.0f, 0.0f));
	matrix_translate(g_env.model, g_env.translate);
	program_setmat4f(g_env.current_glprogram, "model", g_env.model);

	t_matrix	*view = matrix_new_id(4);
	matrix_scale(view, g_env.camera.scale);
	matrix_rotate(view, g_env.camera.rotangle, g_env.camera.rotate);
	matrix_translate(view, g_env.camera.translate);
	program_setmat4f(g_env.current_glprogram, "view", view);
	free(view);

	t_matrix	*projection = matrix_new_perspective(ft_radian(45.0f), (float)g_env.buf.width / (float)g_env.buf.height, 0.1f, 100.0f);
	program_setmat4f(g_env.current_glprogram, "projection", projection);
	free(projection);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_tex_wall.id);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, g_tex_face.id);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	/*
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
*/
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
