#include <stdio.h>
#include <stdlib.h>
#include "cube.h"
#include "texture.h"
#include "libscop.h"
#include "ft_scop.h"
#include <GLFW/glfw3.h>

extern int		g_cube_index;
extern t_cube	*g_cubes[MAX_CUBES];

static t_texture	g_tex_wall = {
	.path = TEXTURES_DIR "wall.ppm",
};

static t_texture	g_tex_face = {
	.path = TEXTURES_DIR "awesomeface.ppm",
};

static t_uniform_val	cube_texture_ratio(void *obj)
{
	t_uniform_val	ret;
	ret.f = ((t_cube*)obj)->texture_ratio;
	return (ret);
}

static t_uniform_val	cube_is_current(void *obj)
{
	t_uniform_val	ret;

	ret.b = (obj == g_cubes[g_cube_index]);
	return (ret);
}

static t_uniform_val	cube_get_offset(void *obj)
{
	t_uniform_val	ret;
	t_model			*model;

	model = ((t_cube *)obj)->obj.model;
	ret.f3[0] = model->offset.x;
	ret.f3[1] = model->offset.y;
	ret.f3[2] = model->offset.z;
	return (ret);
}

static t_uniform const	g_uniforms[] = {
	{
		.type = E_UNIFORM_BOOL,
		.name = "is_selected",
		.resolve = &cube_is_current,
	},
	{
		.type = E_UNIFORM_FLOAT,
		.name = "texture_ratio",
		.resolve = &cube_texture_ratio,
	},
	{
		.type = E_UNIFORM_3FLOAT,
		.name = "_offset",
		.resolve = &cube_get_offset
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

static void	cube_unfocus(void *obj)
{
	t_cube	*cube;

	cube = obj;
	cube->move.x = 0;
	cube->move.y = 0;
}

static void	cube_set_defaults(t_cube *cube)
{
	static t_cube const	cube_defaults = {
		/* .obj = {
		NOTE: the obj member should not be memset. assign all submembers one by
		one if necessary
		}*/
		.move = {
			.x = 0,
			.y = 0,
			.z = 0,
		},
		.texture_ratio = 0.0f,
		.texture_change_axis = 1.0f,
		.texture_toggled = false,
		.rotating = true,
		.unfocus = &cube_unfocus
	};

	memcpy((char*)cube + sizeof(cube->obj), (char const*)&cube_defaults +
			sizeof(cube->obj), sizeof(*cube) - sizeof(cube->obj));
}

static void	cube_init(void *obj)
{
	
	static int	count = 0;
	t_cube	*cube;

	cube = obj;
	fprintf(stdout, "spawning a new cube n%d\n", count);
	cube_set_defaults(cube);
	cube->uid = count++;
	cube->obj.textures = malloc(sizeof(*cube->obj.textures) * 3);
	cube->obj.textures[0] = &g_tex_wall;
	cube->obj.textures[1] = &g_tex_face;
	cube->obj.textures[2] = NULL;
	/* textures probably should not be set there*/
}

static void	cube_update(void *obj, double dtime)
{
	t_cube	*cube;
	float	new_ratio;

	cube = obj;
	if (cube->rotating)
		cube->obj.transform.rotangle += (float)dtime;
	cube->obj.transform.translate.x += cube->move.x * 0.01;
	cube->obj.transform.translate.y += cube->move.y * 0.01;
	/* insert a parameter to make the movement dependent on the elapsed time */
	if (cube->texture_toggled)
	{
		new_ratio = cube->texture_ratio + cube->texture_change_axis * TEXTURING_SPEED;
		if (new_ratio > 1.0f)
		{
			cube->texture_ratio = 1.0f;
			cube->texture_toggled = false;
			cube->texture_change_axis *= -1.0f;
		}
		else if (new_ratio < 0.0f)
		{
			cube->texture_ratio = 0.0f;
			cube->texture_toggled = false;
			cube->texture_change_axis *= -1.0f;
		}
		else
			cube->texture_ratio = new_ratio;
	}
}

void	cube_toggle_texture(t_cube *obj)
{
	if (obj->texture_toggled)
	{
		obj->texture_toggled = false;
		obj->texture_change_axis *= -1.0f;
	}
	else
		obj->texture_toggled = true;
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
			.z = 0.0f,
			.ndim = 3,
		},
		.rotangle = 0,
	}
};
