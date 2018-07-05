/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/23 16:20:25 by tvallee           #+#    #+#             */
/*   Updated: 2018/06/23 17:09:31 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "image.h"
#include "texture.h"
#include "libft/stdbool.h"
#include <glad/glad.h>

extern char const	*ppm_error_msg;

t_bool	texture_init(t_texture *t)
{
	if (!(t->data = ppm_load(t->path, &t->width, &t->height)))
	{
		fprintf(stderr, "while loading texture: %s", ppm_error_msg);
		return (false);
	}
	glGenTextures(1, &t->id);
	glBindTexture(GL_TEXTURE_2D, t->id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t->width, t->height, 0,
			GL_RGB, GL_UNSIGNED_BYTE, t->data);
	glGenerateMipmap(GL_TEXTURE_2D);
	free(t->data);
	return (true);
}

void	texture_deinit(t_texture *t)
{
	glDeleteTextures(1, &t->id);
}
