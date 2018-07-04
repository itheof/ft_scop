/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/21 13:53:47 by tvallee           #+#    #+#             */
/*   Updated: 2018/06/23 14:12:12 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# define _POSIX_C_SOURCE 200809L
# include <unistd.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include "libft/stdbool.h"
# include "file.h"

t_bool	file_open(t_file *dst, char const *path, int oflag)
{
	if ((dst->fd = open(path, oflag)) < 0)
	{
		perror(path);
		return (false);
	}
	if (fstat(dst->fd, &dst->statbuf) < 0)
	{
		perror("fstat");
		close(dst->fd);
		return (false);
	}
	dst->path = path;
	dst->oflag = oflag;
	dst->data = NULL;
	return (true);
}

t_bool	file_load(t_file *f)
{
	if (!(f->statbuf.st_mode & S_IFREG))
		dprintf(2, "warning: cannot load non-regular file %s\n", f->path);
	else if ((f->data = malloc(f->statbuf.st_size + 1)) == NULL)
		perror("malloc");
	else if (read(f->fd, f->data, f->statbuf.st_size) < 0)
		perror("read");
	else
	{
		f->data[f->statbuf.st_size] = '\0';
		return (true);
	}
	return (false);
}

void	file_close(t_file *f)
{
	close(f->fd);
	free(f->data);
}
