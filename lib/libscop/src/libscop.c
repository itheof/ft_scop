#include <glad/glad.h>
#include "libft/stdbool.h"
#include "object.h"

t_bool	libscop_init(void (* (*get_proc_addr)(const char *))(void))
{
#ifndef __APPLE__
	gladLoadGLLoader((GLADloadproc) get_proc_addr);
#endif
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
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
