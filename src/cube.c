#include <stdio.h>
#include <stdlib.h>
#include "cube.h"
#include "libscop.h"
#include <GLFW/glfw3.h>

static t_program	g_program = {
	.gl_loaded = false,
	.vertex = {
		.path = SHADERS_DIR "ft_scop.vs"
	},
	.fragment = {
		.path = SHADERS_DIR "ft_scop.fs"
	}
};

static void	cube_init(void *obj)
{
	static int	count = 0;
	t_cube	*cube;

	cube = obj;
	fprintf(stdout, "spawning a new cube n%d\n", count);
	cube->uid = count++;
}

static void	cube_update(void *obj)
{
	t_cube	*cube;

	cube = obj;
	cube->obj.transform.rotangle = (float)glfwGetTime();
}

t_object	const g_cube_obj = {
	.size = sizeof(t_cube),
	.model_path = NULL,
	.program = &g_program,
	.init = &cube_init,
	.update = &cube_update,
	.transform = {
		.translate = {
			.x = 0,
			.y = 0,
			.ndim = 2,
		},
		.scale = {
			.x = 1,
			.y = 1,
			.ndim = 2,
		},
		.rotate = {
			.x = 0.5f,
			.y = 1.0f,
			.z = 0.0f,
			.ndim = 3,
		},
		.rotangle = 0,
	}
};
