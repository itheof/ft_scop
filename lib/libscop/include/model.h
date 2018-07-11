#ifndef MODEL_H
# define MODEL_H

# include "libft/stdbool.h"

typedef struct	s_model
{
	char const	*path;
	unsigned	uid;
	float const	*vertices;
	size_t		nvertices;
	unsigned	vao;
	unsigned	vbo;
	t_bool		gl_loaded;
}				t_model;

t_model	*model_load(char const *path);
void	model_unload(t_model *m);
#endif
