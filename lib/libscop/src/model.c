#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "model.h"
#include "wfobj.h"
#include "libft/list.h"
#include <glad/glad.h>

#define FALLBACK_MODEL_UID 0

/*
static float const	g_colors[] = {
	0.863f,
	0.827f,
	0.753f,
	0.663f,
	0.502f,
	0.412f,
};
*/

static float const	cube_vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.863,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.863,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.863,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.827,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.827,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.827,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.753,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.753,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.753,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.663,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.663,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.663,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.502,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.502,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.502,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.412,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.412,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.412,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.863,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.863,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.863,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.827,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.827,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.827,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.753,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.753,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.753,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.663,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.663,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.663,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.502,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.502,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.502,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.412,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.412,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.412
};

static t_model	fallback_model = { .path = NULL,
	.uid = FALLBACK_MODEL_UID,
	.vertices = cube_vertices,
	.vertices_size = sizeof(cube_vertices),
	.nvertices = 36,
	.gl_loaded = false
};

static t_bool	transform(t_model *dst, t_wfobj *obj)
{
	(void)obj;
	(void)dst;
	return (true);
}

static t_bool	load(t_model *dst, char const *path)
{
	FILE	*f;
	t_bool	success;
	t_wfobj	*obj;

	if (!(f = fopen(path, "r")))
	{
		fprintf(stderr, "load warning when attempting to read %s: ", path);
		perror("fopen");
		return (false);
	}
	else if (!(obj = wfobj_parse(f)) ||
			!transform(dst, obj))
		success = false;
	else
		success = true;
	wfobj_delete(obj);
	fclose(f);
	return (success);
}

static t_model	*new_from_path_fallback(void)
{
	fprintf(stderr, "model warning: loading fallback cube\n");
	return (&fallback_model);
}

static t_model	*new_from_path(char const *path)
{
	t_model	*ret;

	fprintf(stderr, "model warning: attempting to load %s when not fully implemented yet\n", path);
	if (path != NULL)
	{
		if (!(ret = malloc(sizeof(*ret))))
		{
			perror("load_from_path: malloc");
			return (new_from_path_fallback());
		}
		else if (!load(ret, path))
		{
			free(ret);
			return (new_from_path_fallback());
		}
		else
			return (ret);
	}
	return (new_from_path_fallback());
}

t_model	*model_load(char const *path)
{
	t_model	*ret;

	ret = new_from_path(path);
	if (!ret->gl_loaded)
	{
		glGenBuffers(1, &ret->vbo);
		glGenVertexArrays(1, &ret->vao);
		glBindVertexArray(ret->vao);
		glBindBuffer(GL_ARRAY_BUFFER, ret->vbo);
		glBufferData(GL_ARRAY_BUFFER, ret->vertices_size, ret->vertices,
				GL_STATIC_DRAW);

		/* will depend if the vertices contain UV mapping and other */
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
				6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
				6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE,
				6 * sizeof(float), (void*)(5 * sizeof(float)));
		glEnableVertexAttribArray(2);
		ret->gl_loaded = true;
	}
	return (ret);
}

void	model_unload(t_model *m)
{
    glDeleteVertexArrays(1, &m->vao);
    glDeleteBuffers(1, &m->vbo);
	if (m->uid != FALLBACK_MODEL_UID)
	{

		fprintf(stderr, "model error: attempting to unload model when not implemented yet\n");
	}
}
