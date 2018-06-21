/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_scop.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/21 13:17:01 by tvallee           #+#    #+#             */
/*   Updated: 2018/06/21 15:02:23 by tvallee          ###   ########.fr       */
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
t_bool	shader_init(t_shader *shader);
void	shader_deinit(t_shader *shader);

#endif