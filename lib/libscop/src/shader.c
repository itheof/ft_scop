/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/21 13:38:27 by tvallee           #+#    #+#             */
/*   Updated: 2018/06/21 15:04:25 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <stdio.h>
# include <fcntl.h>
# include "file.h"
# include <glad/glad.h>
# include "shader.h"

t_bool			shader_init(t_shader *shader)
{
	char 	infoLog[512];
	int		success;
	t_file	f;

	if (!file_open(&f, shader->path, O_RDONLY) || !file_load(&f))
	{
		fprintf(stderr, "Warning: could not load shader %s in memory\n",
				shader->path);
		return (false);
	}
	shader->id = glCreateShader(shader->type);
	glShaderSource(shader->id, 1, (char const * const *)&f.data, NULL);
	glCompileShader(shader->id);
	glGetShaderiv(shader->id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader->id, sizeof(infoLog), NULL, infoLog);
		fprintf(stderr, "ERROR compiling shader %s:\n%s\n", shader->path, infoLog);
		file_close(&f);
		return (false);
	}
	file_close(&f);
	return (true);
}

void			shader_deinit(t_shader *shader)
{
	glDeleteShader(shader->id);
}
