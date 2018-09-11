/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   model.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/04 14:17:45 by tvallee           #+#    #+#             */
/*   Updated: 2018/09/11 15:06:40 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODEL_H
# define MODEL_H

# include "libft/stdbool.h"
# include "vector.h"

# define MODEL_DEBUG_VAR "LIBSCOP_MODEL_DEBUG"

typedef struct	s_model
{
	char const			*path;
	unsigned			uid;
	float const			*vertices;
	size_t				vertices_size;
	size_t				nvertices;
	unsigned			vao;
	unsigned			vbo;
	t_bool				gl_loaded;
	t_vector			offset;
	float				scale;
}				t_model;

t_model			*model_load(char const *path);
void			model_unload(t_model *m);
#endif
