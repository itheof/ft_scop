#include "ft_scop.h"

extern t_env	g_env;

static void	scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	(void)window;
	(void)xoffset;
	g_env.camera.translate.z += g_env.camera.translate.z * yoffset * SCROLL_SPEED;
}

static void	toggle_wireframe_mode(void)
{
	g_env.wireframe = !g_env.wireframe;
	if (g_env.wireframe)
	{
		ft_putendl_fd("wireframe on", 2);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		ft_putendl_fd("wireframe off", 2);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if ((key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) ||
			(key == GLFW_KEY_Q && action == GLFW_PRESS))
        glfwSetWindowShouldClose(window, GLFW_TRUE);
	else if (key == GLFW_KEY_W && action == GLFW_PRESS)
		toggle_wireframe_mode();
	else if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		g_env.translate.y = 0;
		g_env.translate.x = 0;
	}
	else if (key == GLFW_KEY_H)
		g_env.camera.translate.x -= 0.05;
	else if (key == GLFW_KEY_J)
		g_env.camera.translate.y -= 0.05;
	else if (key == GLFW_KEY_K)
		g_env.camera.translate.y += 0.05;
	else if (key == GLFW_KEY_L)
		g_env.camera.translate.x += 0.05;
	else
		printf("key: %d scancode %d action %d mods %d\n", key, scancode, action, mods);
}

void	framebuffer_callback(GLFWwindow *window, int width, int height)
{
	(void)window;
	g_env.buf.width = width;
	g_env.buf.height = height;
	glViewport(0, 0, width, height);
}

void 	window_refresh_callback(GLFWwindow* window)
{
    render();
    glfwSwapBuffers(window);
}

void	register_callbacks(void)
{

	glfwSetKeyCallback(g_env.window, key_callback);
	glfwSetScrollCallback(g_env.window, scroll_callback);
	glfwSetFramebufferSizeCallback(g_env.window, framebuffer_callback);
	glfwSetWindowRefreshCallback(g_env.window, window_refresh_callback);
}
