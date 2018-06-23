/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_scop.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/21 13:17:01 by tvallee           #+#    #+#             */
/*   Updated: 2018/06/23 16:25:30 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SCOP_H
# define FT_SCOP_H

# include <ctype.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/stat.h>
# include <stdio.h>
# include <stdlib.h>
# include "libft/stdbool.h"
# include "GLFW/glfw3.h"

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

t_bool	file_open(t_file *dst, char const *path, int oflag);
t_bool	file_load(t_file *f);
void	file_close(t_file *f);

t_bool	program_init(unsigned *id);
void 	program_setb(unsigned int id, char const *name, t_bool value);
void 	program_seti(unsigned int id, char const *name, int value);
void 	program_set2i(unsigned int id, char const *name, int val1, int val2);
void	program_setf(unsigned int id, char const *name, float value);
void	program_set2f(unsigned int id, char const *name, float val1, float val2);

t_bool	shader_init(t_shader *shader);
void	shader_deinit(t_shader *shader);

t_bool	texture_init(t_texture *t);

unsigned char	*ppm_load(char const *path, int *width, int *height);

#endif
