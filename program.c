/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   program.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/21 14:31:02 by tvallee           #+#    #+#             */
/*   Updated: 2018/06/21 15:04:43 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_scop.h"

static t_shader	g_vert = {
	.type = GL_VERTEX_SHADER,
	.path = "ft_scop.vs"
};

static t_shader	g_frag = {
	.type = GL_FRAGMENT_SHADER,
	.path = "ft_scop.fs"
};

t_bool	program_init(unsigned *id)
{
	char 	infoLog[512];
	int		success;

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

void		program_deinit(unsigned id)
{
}
