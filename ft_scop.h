/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_scop.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/21 13:17:01 by tvallee           #+#    #+#             */
/*   Updated: 2018/06/25 18:03:15 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SCOP_H
# define FT_SCOP_H

# define _POSIX_C_SOURCE 200809L
# include <ctype.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "libft/stdbool.h"
# include "libft/libc.h"
# include <glad/glad.h>
# include <GLFW/glfw3.h>

typedef	struct	s_shader
{
	unsigned 	id;
	GLenum		type;
	char const	*path;
}				t_shader;

typedef struct	s_texture
{
	unsigned		id;
	char const		*path;
	unsigned char	*data;
	int				width;
	int				height;
}				t_texture;

typedef struct	s_file
{
	struct stat	statbuf;
	int			fd;
	int			oflag;
	char const	*path;
	char		*data;
}				t_file;

typedef struct	s_vector
{
	size_t		ndim;
	float		x;
	float		y;
	float		z;
	float		w;
}				t_vector;

typedef struct	s_matrix
{
	size_t	xlen;
	size_t	ylen;
	size_t	nelem;
	float	elems[];
}				t_matrix;

t_bool	file_open(t_file *dst, char const *path, int oflag);
t_bool	file_load(t_file *f);
void	file_close(t_file *f);

t_bool	program_init(unsigned *id);
void 	program_setb(unsigned int id, char const *name, t_bool value);
void 	program_seti(unsigned int id, char const *name, int value);
void 	program_set2i(unsigned int id, char const *name, int val1, int val2);
void	program_setf(unsigned int id, char const *name, float value);
void	program_set2f(unsigned int id, char const *name, float val1, float val2);
void	program_setmat4f(unsigned int id, char const *name, t_matrix const *mat);
void	program_deinit(unsigned id);

t_bool	shader_init(t_shader *shader);
void	shader_deinit(t_shader *shader);

t_bool	texture_init(t_texture *t);
void	texture_deinit(t_texture *t);

unsigned char	*ppm_load(char const *path, int *width, int *height);

t_matrix	*matrix_new(t_vector size);
t_matrix	*matrix_new_id(size_t len);

void		matrix_id(t_matrix *m);
void		matrix_translate(t_matrix *m, t_vector trans);
void		matrix_scale(t_matrix *m, t_vector scale);

void		matrix_set(t_matrix *matrix, t_vector pos, float val);
void		matrix_dump(t_matrix const *matrix);
t_matrix	*matrix_mult(t_matrix const *left, t_matrix const *right);

t_vector	vec1(float x);
t_vector	vec2(float x, float y);
t_vector	vec3(float x, float y, float z);
t_vector	vec4(float x, float y, float z, float w);
t_matrix	*vec_to_matrix(t_vector vec);

#endif
