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

static unsigned int	VAO = 0;
static unsigned int	VBO = 0;

t_env	g_env = {
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

t_texture g_tex_wall = {
	.path = TEXTURES_DIR "wall.ppm",
};

t_texture g_tex_face = {
	.path = TEXTURES_DIR "awesomeface.ppm",
};

void	render(void);

void 	__attribute__ ((noreturn)) cleanup()
{
	program_deinit(g_env.current_glprogram);
	texture_deinit(&g_tex_wall);
	texture_deinit(&g_tex_face);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
	glfwTerminate();
	exit(0);
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
}

int		main(void)
{
	if (!init(&g_env))
		return (-1);

    while (!glfwWindowShouldClose(g_env.window))
    {
		render();
        glfwSwapBuffers(g_env.window);
        glfwPollEvents();
    }
	cleanup();
}
