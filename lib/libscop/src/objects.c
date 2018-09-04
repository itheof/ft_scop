# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <assert.h>
#include "matrix.h"
#include "object.h"
#include "uniform.h"
# include <glad/glad.h>

extern t_matrix	*g_projection;
static t_object	*g_objectsl = NULL;

static char	tx_uniforms[][sizeof("texture15")] = {
	"_texture0",
	"_texture1",
	"_texture2",
	"_texture3",
	"_texture4",
	"_texture5",
	"_texture6",
	"_texture7",
	"_texture8",
	"_texture9",
	"_texture10",
	"_texture11",
	"_texture12",
	"_texture13",
	"_texture14",
	"_texture15"
};

void	*objects_push(t_object const *obj)
{
	void		*ret;
	t_object	*tmp;
	size_t	i;

	assert(obj->size > sizeof(*obj));
	if (!(ret = malloc(obj->size)))
	{
		fprintf(stderr, "objects error: failed allocating %zu bytes\n", obj->size);
		return (NULL);
	}
	tmp = ret;
	memcpy(tmp, obj, sizeof(*obj));
	tmp->next = g_objectsl;
	g_objectsl = tmp;

	if (tmp->init)
		tmp->init(tmp);

	tmp->model = model_load(tmp->model_path);
	if (!program_init(tmp->program))
	{
		//TODO: error handling
	}
	i = 0;
	while (tmp->textures[i])
	{
		if (!texture_init(tmp->textures[i]))
		{
			fprintf(stderr, "texture_init failed\n");
			;//TODO: error handling
		}
		else
			fprintf(stderr, "loaded new texture\n");
		i++;
	}
	return (ret);
}

	/* model_load -> model_bind so we can count references to them ?
	 * XXX: Do not try to load two objects with same model !! */

void	objects_pop(t_object *obj)
{
	size_t	i;

	program_deinit(obj->program);
	i = 0;
	while (obj->textures[i])
		texture_deinit(obj->textures[i++]);
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
	size_t			i;

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
			//program_seti(g_objectsl->program, "texture1", g_objectsl->textures[0]->id);
			//program_seti(g_objectsl->program, "texture2", g_objectsl->textures[1]->id);
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
		matrix_id(model);
		matrix_scale(model, object->transform.scale);
		matrix_translate(model, object->transform.translate);
		matrix_rotate(model, object->transform.rotangle, object->transform.rotate);
		program_setmat4f(object->program, "_model", model);
		program_setmat4f(object->program, "_view", view);
		program_setmat4f(object->program, "_projection", g_projection);

		/* setting program uniforms */

		if (object->program->uniforms != NULL)
		{
			t_uniform const *current;

			current = *object->program->uniforms;
			while (current->type != E_UNIFORM_END)
			{
				switch (current->type)
				{
					case E_UNIFORM_BOOL:
						program_setb(object->program, current->name, current->resolve(object).b);
						break;
					case E_UNIFORM_FLOAT:
						program_setf(object->program, current->name, current->resolve(object).f);
						break;
					default:
						fprintf(stderr, "unsupported yet uniform type %d\n", current->type);
						abort();
				}
				current++;
			}
		}

		/* done */

		i = 0;
		while (object->textures[i])
		{
			assert(i < 16);
			/* XXX: not guaranteed that gl supports I > 15 aka 16 textures*/
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, object->textures[i]->id);
			program_seti(object->program, tx_uniforms[i], i);
			i++;
		}

		glBindVertexArray(object->model->vao);
		glDrawArrays(GL_TRIANGLES, 0, object->model->nvertices);
		object = object->next;
	}
	/*deinit matrices on last render*/
}
