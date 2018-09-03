/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libscop.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 14:00:28 by tvallee           #+#    #+#             */
/*   Updated: 2018/09/03 14:03:25 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <glad/glad.h>
#include "libft/stdbool.h"
#include "object.h"
#include <stdio.h>

t_bool	libscop_init(void (* (*get_proc_addr)(const char *))(void))
{
	if (!gladLoadGLLoader((GLADloadproc)get_proc_addr))
    {
		fprintf(stderr, "Failed to initialize GLAD\n");
        return (false);
    }
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
/*	if (!program_init(&g_program))
	{
		ft_putendl_fd("FATAL: failed to compile shaders", 2);
		return (false);
	}*/
	return (true);
}

void	libscop_deinit(void)
{
	objects_cleanup();
/*	texture_deinit(&g_tex_wall);
	texture_deinit(&g_tex_face);*/
}
