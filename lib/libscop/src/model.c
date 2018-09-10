#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "model.h"
#include "wfobj.h"
#include "libft/list.h"
#include <glad/glad.h>

#define FALLBACK_MODEL_UID 0

static float const	g_colors[] = {
	0.863f,
	0.827f,
	0.753f,
	0.663f,
	0.502f,
	0.412f,
	0.2f,
};

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
	.gl_loaded = false,
	.offset = {
		.ndim = 3,
		.x = 0,
		.y = 0,
		.z = 0
	}
};

static void		fill_array_with_list(void *a, t_list *lst)
{
	size_t	i;

	i = 0;
	while (lst)
	{
		memcpy(a, lst->content, lst->content_size);
		a = (char*)a + lst->content_size;
		lst = lst->next;
		i++;
	}
	fprintf(stderr, "copied %zu elems\n", i );
}

static void		transform_triangle(float *dst, t_face *f, int first_vertice, t_vertex *v_a, t_vertex_normal *vn_a, t_vertex_texture *vt_a, unsigned int colori)
{
	float		color;

	color = g_colors[colori];
	(void)vn_a;
	memcpy(dst, v_a[f->triplets[first_vertice][TRIPLET_V] - 1].coord, 3 * sizeof(float));
	if (f->texture)
		memcpy(dst + 3, vt_a[f->triplets[first_vertice][TRIPLET_VT] - 1].coord, 2 * sizeof(float));
	else
	{
		dst[3] = 0.0f;
		dst[4] = 0.0f;
	}
	dst[5] = color;
	memcpy(dst + 6, v_a[f->triplets[first_vertice + 1][TRIPLET_V] - 1].coord, 3 * sizeof(float));
	if (f->texture)
		memcpy(dst + 9, vt_a[f->triplets[first_vertice + 1][TRIPLET_VT] - 1].coord, 2 * sizeof(float));
	else
	{
		dst[9] = 0.0f;
		dst[10] = 0.0f;
	}
	dst[11] = color;
	memcpy(dst + 12, v_a[f->triplets[(first_vertice + 2) % 4][TRIPLET_V] - 1].coord, 3 * sizeof(float));
	if (f->texture)
		memcpy(dst + 15, vt_a[f->triplets[(first_vertice + 2) % 4][TRIPLET_VT] -1].coord, 2 *sizeof(float));
	else
	{
		dst[15] = 0.0f;
		dst[16] = 0.0f;
	}
	dst[17] = color;
}

static float	*transform_face(float *dst, t_face *f, t_vertex *v_a, t_vertex_normal *vn_a, t_vertex_texture *vt_a)
{
	static unsigned int	colori = 0;


	transform_triangle(dst, f, 0, v_a, vn_a, vt_a, colori % 6);
	dst += 18;
	//fprintf(stderr, "hmmm ndim: %d, %d %d %d %d", f->);
	if (f->n_sides == 4)
	{
		transform_triangle(dst, f, 2, v_a, vn_a, vt_a, colori % 6);
		dst += 18;
	}
	colori++;
	return (dst);
}

static t_bool	transform(t_model *dst, t_wfobj *obj)
{
	t_vertex			*v_a;
	t_vertex_normal		*vn_a;
	t_vertex_texture	*vt_a;
	static int			new_uid = FALLBACK_MODEL_UID + 1;

	v_a = malloc(sizeof(*v_a) * obj->n_v);
	fprintf(stderr, "malloced %zu bytes at %p\n", sizeof(*v_a) * obj->n_v, (void*)v_a);
	vn_a = malloc(sizeof(*vn_a) * obj->n_vn);
	vt_a = malloc(sizeof(*vt_a) * obj->n_vt);
	if (!v_a || !vn_a || !vt_a)
	{
		free(v_a);
		free(vn_a);
		free(vt_a);
		perror("transform malloc");
		return (false);
	}
	fill_array_with_list(v_a, obj->v);
	fill_array_with_list(vn_a, obj->vn);
	fill_array_with_list(vt_a, obj->vt);

	/* count how many triangles */
	t_list	*current;
	size_t	n;

	n = 0;
	current = obj->f;
	while (current)
	{
		if (((t_face *)current->content)->n_sides == 4)
			n += 2;
		else
			n++;
		current = current->next;
	}

	dst->gl_loaded = false;
	dst->nvertices = n * 3;
	dst->vertices_size = sizeof(float) * dst->nvertices * 6;
	if (!(dst->vertices = malloc(dst->vertices_size)))
	{
		free(v_a);
		free(vn_a);
		free(vt_a);
		perror("transform malloc");
		return (false);
	}
	dst->uid = new_uid++;

	n = 0;
	current = obj->f;
	float	*tmp;
	tmp = (float *)dst->vertices;
	while (current)
	{
		tmp = transform_face(tmp, current->content, v_a, vn_a, vt_a);
		current = current->next;
	}

	dst->offset.ndim = 3;
	dst->offset.x = - (obj->max_vertex.coord[0] - obj->min_vertex.coord[0]) / 2;
	dst->offset.y = - (obj->max_vertex.coord[1] - obj->min_vertex.coord[1]) / 2;
	dst->offset.z = - (obj->max_vertex.coord[2] - obj->min_vertex.coord[2]) / 2;

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
