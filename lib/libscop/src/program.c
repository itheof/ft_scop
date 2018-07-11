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

#include <stdio.h>
#include "shader.h"
#include "program.h"
# include <glad/glad.h>

t_bool	program_init(t_program *p)
{
	char 	infoLog[512];
	int	success;

	p->vertex.type = GL_VERTEX_SHADER;
	p->fragment.type = GL_FRAGMENT_SHADER;
	if (!shader_init(&p->vertex) || !shader_init(&p->fragment))
		return (false);
	p->id = glCreateProgram();
	glAttachShader(p->id, p->vertex.id);
	glAttachShader(p->id, p->fragment.id);
	glLinkProgram(p->id);
	glGetProgramiv(p->id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(p->id, 512, NULL, infoLog);
		fprintf(stderr, "ERROR compiling program: %s\n", infoLog);
	}
	shader_deinit(&p->vertex);
	shader_deinit(&p->fragment);
	return ((success) != false);
}

void 	program_setb(t_program *p, char const *name, t_bool value)
{
    glUniform1i(glGetUniformLocation(p->id, name), value);
}

void 	program_seti(t_program *p, char const *name, int value)
{
    glUniform1i(glGetUniformLocation(p->id, name), value);
}

void 	program_set2i(t_program *p, char const *name, int val1, int val2)
{
    glUniform2i(glGetUniformLocation(p->id, name), val1, val2);
}

void	program_setf(t_program *p, char const *name, float value)
{
    glUniform1f(glGetUniformLocation(p->id, name), value);
}

void	program_set2f(t_program *p, char const *name, float val1, float val2)
{
    glUniform2f(glGetUniformLocation(p->id, name), val1, val2);
}

void	program_setmat4f(t_program *p, char const *name, t_matrix const *mat)
{
	unsigned int loc;
   
	loc = glGetUniformLocation(p->id, name);
	glUniformMatrix4fv(loc, 1, GL_TRUE, mat->elems);
}

void	program_deinit(t_program *p)
{
	glDeleteProgram(p->id);
}
