#include <stdio.h>
#include <string.h>
#include "model.h"
#include <glad/glad.h>

#define FALLBACK_MODEL_UID 0

static float const cube_vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

static t_model	fallback_model = {
	.path = NULL,
	.uid = FALLBACK_MODEL_UID,
	.vertices = cube_vertices,
	.nvertices = sizeof(cube_vertices) / sizeof(*cube_vertices),
	.gl_loaded = false
};

t_model	*model_load(char const *path)
{
	t_model	*ret;

	if (path != NULL)
	{
		fprintf(stderr, "model warning: attempting to load %s when not implemented yet\n", path);
	}
	ret = &fallback_model;
	if (!ret->gl_loaded)
	{
		glGenBuffers(1, &ret->vbo);
		glGenVertexArrays(1, &ret->vao);
		glBindVertexArray(ret->vao);
		glBindBuffer(GL_ARRAY_BUFFER, ret->vbo);
		glBufferData(GL_ARRAY_BUFFER, ret->nvertices, ret->vertices,
				GL_STATIC_DRAW);

		/* will depend if the vertices contain UV mapping and other */
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
				5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
				5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
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
