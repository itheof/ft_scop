#include "ft_scop.h"

extern  t_texture g_tex_wall;
extern  t_texture g_tex_face;

t_bool	init_shaders(t_env *env)
{
	if (!program_init(&env->current_glprogram))
		return (false);
	return (true);
}

static void error_callback(int error, const char* description)
{
	(void)error;
	fprintf(stderr, "GLFW error (%d): %s\n", error, description);
}

t_bool	init(t_env *env)
{
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
        return (false);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    env->window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!env->window)
    {
		ft_putstr_fd("Could not open window", 2);
        glfwTerminate();
        return (false);
    }
    glfwMakeContextCurrent(env->window);
#ifndef __APPLE__
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
#endif
	if (!init_shaders(env))
	{
		ft_putendl_fd("Failed to compile shaders", 2);
		glfwTerminate();
		return (false);
	}
	if (!texture_init(&g_tex_wall) || !texture_init(&g_tex_face))
	{
		ft_putendl_fd("Failed to load textures", 2);
	}
	glfwGetFramebufferSize(env->window, &env->buf.width, &env->buf.height);
	glViewport(0, 0, env->buf.width, env->buf.height);
	glfwSwapInterval(1);
	register_callbacks();
	if (!(env->model = matrix_new_id(4)))
	{
		/*do the twist*/
		;
	}
	return (true);
}
