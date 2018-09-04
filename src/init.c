#include "ft_scop.h"

extern t_texture 	g_tex_wall;
extern t_texture 	g_tex_face;

static void 	error_callback(int error, const char* description)
{
	(void)error;
	fprintf(stderr, "GLFW error (%d): %s\n", error, description);
}

t_bool	init(t_env *env)
{
	int	width;
	int	height;

	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
        return (false);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    env->window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT,"Hello World", NULL, NULL);
    if (!env->window)
    {
		ft_putstr_fd("Could not open window", 2);
        glfwTerminate();
        return (false);
    }
    glfwMakeContextCurrent(env->window);
	if (!libscop_init(glfwGetProcAddress))
	{
		glfwTerminate();
		return (false);
	}
	glfwGetFramebufferSize(env->window, &width, &height);
	display_set_viewport(width, height);
	glfwSwapInterval(1);
	register_callbacks();
	/*
	if (!texture_init(&g_tex_wall) || !texture_init(&g_tex_face))
	{
		ft_putendl_fd("Failed to load textures", 2);
	}*/
	return (true);
}
