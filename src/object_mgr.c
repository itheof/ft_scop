/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_mgr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/05 15:36:01 by tvallee           #+#    #+#             */
/*   Updated: 2018/09/11 17:05:54 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_scop.h"

int		g_cube_index = -1;
t_cube	*g_cubes[MAX_CUBES] = { NULL };

t_cube	*cube_get_current(void)
{
	if (g_cube_index == -1)
		return (NULL);
	return (g_cubes[g_cube_index]);
}

void	cube_focus_next(void)
{
	if (g_cube_index == -1)
		return ;
	else if (g_cube_index == MAX_CUBES - 1)
	{
		g_cubes[g_cube_index]->unfocus(g_cubes[g_cube_index]);
		g_cube_index = 0;
	}
	else if (g_cubes[g_cube_index + 1])
	{

		g_cubes[g_cube_index]->unfocus(g_cubes[g_cube_index]);
		g_cube_index++;
	}
	else
	{
		g_cubes[g_cube_index]->unfocus(g_cubes[g_cube_index]);
		g_cube_index = 0;
	}
	/* the hook should not be called when we dont change the selected cube*/
}

void	cube_focus_prev(void)
{
	if (g_cube_index < 0)
		return ;
	else if (g_cube_index == 0)
	{
		g_cubes[g_cube_index]->unfocus(g_cubes[g_cube_index]);
		while (g_cube_index + 1 < MAX_CUBES && g_cubes[g_cube_index + 1])
			g_cube_index++;
	}
	else
	{
		g_cubes[g_cube_index]->unfocus(g_cubes[g_cube_index]);
		g_cube_index--;
	}
	g_cubes[g_cube_index]->unfocus(g_cubes[g_cube_index]);
}

void	push_cube(char const *model_path)
{
	t_object	newmodel;

	fprintf(stderr, "spawning new %s object\n", (model_path) ? model_path : "fallback");
	memcpy(&newmodel, &g_cube_obj, sizeof(newmodel));
	newmodel.model_path = model_path;
	if (g_cube_index + 1 >= MAX_CUBES)
	{
		fprintf(stderr, "already too many objects. skipping model %s...\n", model_path);
		return ;
	}
	g_cube_index++;
	g_cubes[g_cube_index] = objects_push(&newmodel);
}
