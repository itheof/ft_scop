/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   program.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/21 14:31:02 by tvallee           #+#    #+#             */
/*   Updated: 2018/06/21 14:50:40 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_scop.h"

static t_shader	g_vert = {
	.type = GL_VERTEX_SHADER,
	.path = SHADERS_DIR "ft_scop.vs"
};

static t_shader	g_frag = {
	.type = GL_FRAGMENT_SHADER,
	.path = SHADERS_DIR "ft_scop.fs"
};

t_bool	program_init(unsigned *id)
{
	char 	infoLog[512];
	int	success;

	if (!shader_init(&g_vert) || !shader_init(&g_frag))
		return (false);
	*id = glCreateProgram();
	glAttachShader(*id, g_vert.id);
	glAttachShader(*id, g_frag.id);
	glLinkProgram(*id);
	glGetProgramiv(*id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(*id, 512, NULL, infoLog);
		dprintf(2, "ERROR compiling program: %s\n", infoLog);
	}
	shader_deinit(&g_vert);
	shader_deinit(&g_frag);
	return ((success) != false);
}

void 	program_setb(unsigned int id, char const *name, t_bool value)
{
    glUniform1i(glGetUniformLocation(id, name), value);
}

void 	program_seti(unsigned int id, char const *name, int value)
{
    glUniform1i(glGetUniformLocation(id, name), value);
}

void 	program_set2i(unsigned int id, char const *name, int val1, int val2)
{
    glUniform2i(glGetUniformLocation(id, name), val1, val2);
}

void	program_setf(unsigned int id, char const *name, float value)
{
    glUniform1f(glGetUniformLocation(id, name), value);
}

void	program_set2f(unsigned int id, char const *name, float val1, float val2)
{
    glUniform2f(glGetUniformLocation(id, name), val1, val2);
}

void	program_setmat4f(unsigned int id, char const *name, t_matrix const *mat)
{
	unsigned int loc;
   
	loc = glGetUniformLocation(id, name);
	glUniformMatrix4fv(loc, 1, GL_TRUE, mat->elems);
}

void	program_deinit(unsigned id)
{
	glDeleteProgram(id);
}
