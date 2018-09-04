#include <stdio.h>
#include <stdlib.h>
#include "cube.h"
#include "texture.h"
#include "libscop.h"
#include "ft_scop.h"
#include <GLFW/glfw3.h>

static t_texture	g_tex_wall = {
	.path = TEXTURES_DIR "wall.ppm",
};

static t_texture	g_tex_face = {
	.path = TEXTURES_DIR "awesomeface.ppm",
};

static t_uniform const	g_uniforms[] = {
	{
		.type = E_UNIFORM_BOOL,
		.name = "is_selected",
		.resolve = &cube_is_current,
	},
	{
		.type = E_UNIFORM_END,
	}
};

static t_program	g_program = {
	.gl_loaded = false,
	.vertex = {
		.path = SHADERS_DIR "ft_scop.vs"
	},
	.fragment = {
		.path = SHADERS_DIR "ft_scop.fs"
	},
	.uniforms = &g_uniforms,
};

static void	cube_init(void *obj)
{
	static int	count = 0;
	t_cube	*cube;

	cube = obj;
	fprintf(stdout, "spawning a new cube n%d\n", count);
	cube->uid = count++;
	cube->obj.textures = malloc(sizeof(*cube->obj.textures) * 3);
	cube->obj.textures[0] = &g_tex_wall;
	cube->obj.textures[1] = &g_tex_face;
	cube->obj.textures[2] = NULL;

	cube->move.x = 0;
	cube->move.y = 0;
	cube->move.z = 0;
}

static void	cube_update(void *obj)
{
	t_cube	*cube;

	cube = obj;
	cube->obj.transform.rotangle = (float)glfwGetTime();
	cube->obj.transform.translate.x += cube->move.x * 0.01;
	cube->obj.transform.translate.y += cube->move.y * 0.01;
	/* insert a parameter to make the movement dependent on the elapsed time */
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
			.x = 0.0f,
			.y = 1.0f,
			.z = 1.0f,
			.ndim = 3,
		},
		.rotangle = 0,
	}
};
