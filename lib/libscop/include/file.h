/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/04 14:16:09 by tvallee           #+#    #+#             */
/*   Updated: 2018/09/04 14:16:29 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_H
# define FILE_H

# include <unistd.h>
# include <sys/stat.h>
# include <sys/types.h>
# include "libft/stdbool.h"

# define ASSETS_DIR "assets/"

typedef struct	s_file
{
	struct stat	statbuf;
	int			fd;
	int			oflag;
	char const	*path;
	char		*data;
}				t_file;

t_bool			file_open(t_file *dst, char const *path, int oflag);
t_bool			file_load(t_file *f);
void			file_close(t_file *f);

#endif
