/* enable textures 
		program_seti(&g_env.current_glprogram, "texture1", 0);
		program_seti(&g_env.current_glprogram, "texture2", 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_tex_wall.id);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, g_tex_face.id);
*/
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <assert.h>
#include "matrix.h"
#include "object.h"
# include <glad/glad.h>

extern t_matrix	*g_projection;
static t_object	*g_objectsl = NULL;

void	*objects_push(t_object const *obj)
{
	void		*ret;
	t_object	*tmp;

	assert(obj->size > sizeof(*obj));
	if (!(ret = malloc(obj->size)))
	{
		fprintf(stderr, "objects error: failed allocating %zu bytes\n", obj->size);
		return (NULL);
	}
	tmp = ret;
	tmp->next = g_objectsl;
	g_objectsl = tmp;
	memcpy(tmp, obj, sizeof(*obj));

	if (tmp->init)
		tmp->init(tmp);

	tmp->model = model_load(tmp->model_path);
	if (!program_init(tmp->program))
	{
		//TODO: error handling
	}
	return (ret);
}

	/* model_load -> model_bind so we can count references to them ?
	 * XXX: Do not try to load two objects with same model !! */

void	objects_pop(t_object *obj)
{
	program_deinit(obj->program);
	model_unload(obj->model); //XXX
	if (obj->prev)
	{
		obj->prev->next = obj->next;
		if (obj->next)
			obj->next->prev = obj->prev;
	}
	else
	{
		if (obj->next)
			obj->next->prev = NULL;
		g_objectsl = obj->next;
	}
	free(obj);
}

void	objects_cleanup(void)
{
	while (g_objectsl != NULL)
		objects_pop(g_objectsl);
}

void	objects_update(void)
{
	t_object	*object;

	object = g_objectsl;
	while (object != NULL)
	{
		object->update(object);
		object = object->next;
	}
}

void	objects_render(t_camera camera)
{
	static t_matrix	*view = NULL;
	static t_matrix	*model = NULL;
	t_object		*object;

	if (!view)
	{
		view = matrix_new(vec2(4, 4));
		model = matrix_new(vec2(4, 4));
		if (!view || !model)
		{
			perror("malloc");
			fprintf(stderr, "display error: could not allocate enough memory\n");
			free(view);
			view = NULL;
			free(model);
			model = NULL;
			return ;
		}
	}
	if (!g_projection)
	{
		fprintf(stderr, "render error: projection matrix is null");
		return ;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	matrix_id(view);
	matrix_scale(view, camera.transform.scale);
	matrix_rotate(view, camera.transform.rotangle, camera.transform.rotate);
	matrix_translate(view, camera.transform.translate);
	object = g_objectsl;
	while (object != NULL)
	{
		glUseProgram(object->program->id);
		matrix_scale(model, object->transform.scale);
		matrix_rotate(model, object->transform.rotangle, object->transform.rotate);
		matrix_translate(model, object->transform.translate);
		program_setmat4f(object->program, "model", model);
		program_setmat4f(object->program, "view", view);
		program_setmat4f(object->program, "projection", g_projection);
		glBindVertexArray(object->model->vao);
		fprintf(stderr, "%d\n", (int)object->model->vao);
		glDrawArrays(GL_TRIANGLES, 0, object->model->nvertices);
		object = object->next;
	}
	/*deinit matrices on last render*/
}
