#include <stdio.h>
#include "ft_scop.h"
#include "cube.h"

extern t_env	g_env;
extern t_cube	*g_cube;

static void	scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	(void)window;
	(void)xoffset;
	g_env.camera.transform.translate.z += g_env.camera.transform.translate.z * yoffset * SCROLL_SPEED;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if ((key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) ||
			(key == GLFW_KEY_Q && action == GLFW_PRESS))
        glfwSetWindowShouldClose(window, GLFW_TRUE);
	else if (key == GLFW_KEY_W && action == GLFW_PRESS)
		display_toggle_wireframe();
	else if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		/* This can be enhanced by keeping only a pointer to the obj header.
		 * Then we will be able to memset the transform data easily. We can
		 * even specify a function pointer to reset the
		 * object (or call init again)
		*/
		g_cube->obj.transform.translate.y = 0;
		g_cube->obj.transform.translate.x = 0;
	}
	else if (key == GLFW_KEY_H)
		g_env.camera.transform.translate.x -= 0.05;
	else if (key == GLFW_KEY_J)
		g_env.camera.transform.translate.y -= 0.05;
	else if (key == GLFW_KEY_K)
		g_env.camera.transform.translate.y += 0.05;
	else if (key == GLFW_KEY_L)
		g_env.camera.transform.translate.x += 0.05;
	else if (key == GLFW_KEY_MINUS)
		g_env.camera.transform.translate.z -= SCROLL_SPEED;
	else if (key == GLFW_KEY_EQUAL)
		g_env.camera.transform.translate.z += SCROLL_SPEED;
	else
		printf("key: %d scancode %d action %d mods %d\n", key, scancode, action, mods);
}

void	framebuffer_callback(GLFWwindow *window, int width, int height)
{
	(void)window;
	display_set_viewport(width, height);
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
	glfwSetFramebufferSizeCallback(g_env.window, framebuffer_callback);
	glfwSetWindowRefreshCallback(g_env.window, window_refresh_callback);
}
