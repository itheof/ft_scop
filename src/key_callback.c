/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_callback.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/11 11:52:05 by tvallee           #+#    #+#             */
/*   Updated: 2018/09/11 17:20:56 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_scop.h"

extern t_env	g_env;

void		key_callback(GLFWwindow *window,
		int key, int scancode, int action, int mods)
{
	t_cube	*focused;

	(void)scancode;
	focused = cube_get_current();
	switch (key) {
		case GLFW_KEY_ESCAPE:
		case GLFW_KEY_Q:
			if (action == GLFW_PRESS)
				glfwSetWindowShouldClose(window, GLFW_TRUE);
			break ;
		case GLFW_KEY_W:
			if (action == GLFW_PRESS)
				display_toggle_wireframe();
			break ;
		case GLFW_KEY_MINUS:
			g_env.camera.transform.translate.z -= SCROLL_SPEED;
			break ;
		case GLFW_KEY_EQUAL:
			g_env.camera.transform.translate.z += SCROLL_SPEED;
			break ;
		case GLFW_KEY_ENTER:
			if (action == GLFW_PRESS)
			{
				if (focused)
					focused->unfocus(focused);
				push_cube(NULL);
			}
			break ; 
		default:
			if (!(focused))
				return ;
			switch (key) {
				case GLFW_KEY_R:
					if (action == GLFW_PRESS)
					{
						focused->obj.transform.translate.y = 0;
						focused->obj.transform.translate.x = 0;
					}
					break ;
				case GLFW_KEY_TAB:
					if (action == GLFW_PRESS)
					{
						fprintf(stderr, "plop %s\n", (mods & GLFW_MOD_SHIFT) ? "prev" : "next");
						if (mods & GLFW_MOD_SHIFT)
							cube_focus_prev();
						else
							cube_focus_next();
					}
					break ;
				case GLFW_KEY_SPACE:
					if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
						focused->rotating = !focused->rotating;
					break ;
				case GLFW_KEY_H:
					focused->move.x = (action == GLFW_RELEASE) ? 0 : -1.0;
					break ;
				case GLFW_KEY_J:
					focused->move.y = (action == GLFW_RELEASE) ? 0 : -1.0;
					break ;
				case GLFW_KEY_K:
					focused->move.y = (action == GLFW_RELEASE) ? 0 : 1.0;
					break ;
				case GLFW_KEY_L:
					focused->move.x = (action == GLFW_RELEASE) ? 0 : 1.0;
					break ;
				case GLFW_KEY_T:
					if (action == GLFW_PRESS)
						cube_toggle_texture(focused);
					break;
			}
			break ;
	}
}
