/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_callback.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/11 11:52:05 by tvallee           #+#    #+#             */
/*   Updated: 2018/09/11 11:54:12 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_scop.h"

extern t_env	g_env;
extern int		g_cube_index;
extern t_cube	*g_cubes[MAX_CUBES];

void		key_callback(GLFWwindow *window,
		int key, int scancode, int action, int mods)
{
	t_cube	*focused;

	(void)scancode;
	if ((key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) ||
			(key == GLFW_KEY_Q && action == GLFW_PRESS))
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	else if (key == GLFW_KEY_W && action == GLFW_PRESS)
		display_toggle_wireframe();
	else if (key == GLFW_KEY_MINUS)
		g_env.camera.transform.translate.z -= SCROLL_SPEED;
	else if (key == GLFW_KEY_EQUAL)
		g_env.camera.transform.translate.z += SCROLL_SPEED;
	else if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
	{
		focused = g_cubes[g_cube_index];
		if (focused)
			focused->unfocus(focused);
		push_cube(NULL);
	}
	else if (g_cubes[g_cube_index])
	{
		focused = g_cubes[g_cube_index];
		if (key == GLFW_KEY_R && action == GLFW_PRESS)
		{
			//NOTE: this has no effect atm
			focused->obj.transform.translate.y = 0;
			focused->obj.transform.translate.x = 0;
		}
		else if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
		{
			if (mods & GLFW_MOD_SHIFT)
				cube_focus_prev();
			else
				cube_focus_next();
		}
		else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
			focused->rotating = !focused->rotating;
		else if (key == GLFW_KEY_H)
			focused->move.x = (action == GLFW_RELEASE) ? 0 : -1.0;
		else if (key == GLFW_KEY_J)
			focused->move.y = (action == GLFW_RELEASE) ? 0 : -1.0;
		else if (key == GLFW_KEY_K)
			focused->move.y = (action == GLFW_RELEASE) ? 0 : 1.0;
		else if (key == GLFW_KEY_L)
			focused->move.x = (action == GLFW_RELEASE) ? 0 : 1.0;
		else if (key == GLFW_KEY_T && action == GLFW_PRESS)
			cube_toggle_texture(focused);
	}
}
