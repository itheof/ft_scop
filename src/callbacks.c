#include <stdio.h>
#include "ft_scop.h"
#include "cube.h"

extern t_env	g_env;
extern int		g_cube_index;
extern t_cube	*g_cubes[MAX_CUBES];

static void	scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	(void)window;
	(void)xoffset;
	g_env.camera.transform.translate.z += g_env.camera.transform.translate.z * yoffset * SCROLL_SPEED;
}

static void	mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	(void)window;
	(void)button;
	(void)mods;
	g_env.mouse_held = (action == GLFW_PRESS);
	update_camera(true);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
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
			/* This can be enhanced by keeping only a pointer to the obj header.
			 * Then we will be able to memset the transform data easily. We can
			 * even specify a function pointer to reset the
			 * object (or call init again)
			*/
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
/*	else if (action == GLFW_PRESS)
		printf("key: %d scancode %d action %d mods %d\n", key, scancode, action, mods);
		*/
}

void	framebuffer_callback(GLFWwindow *window, int width, int height)
{
	(void)window;
	display_set_viewport(width, height);
	g_env.width = width;
	g_env.height = height;
}

void 	window_refresh_callback(GLFWwindow* window)
{
    objects_render(g_env.camera);
    glfwSwapBuffers(window);
}

void	register_callbacks(void)
{
	glfwSetKeyCallback(g_env.window, key_callback);
	glfwSetScrollCallback(g_env.window, scroll_callback);
	glfwSetMouseButtonCallback(g_env.window, mouse_button_callback);
	glfwSetFramebufferSizeCallback(g_env.window, framebuffer_callback);
	glfwSetWindowRefreshCallback(g_env.window, window_refresh_callback);
}
