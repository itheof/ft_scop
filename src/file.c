/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/21 13:53:47 by tvallee           #+#    #+#             */
/*   Updated: 2018/06/21 14:26:40 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_scop.h"

t_bool	file_open(t_file *dst, char const *path, int oflag)
{
	struct stat	buf;

	if ((dst->fd = open(path, oflag)) < 0)
	{
		perror("open");
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
}

t_bool	file_load(t_file *f)
{
	if (!(f->statbuf.st_mode & S_IFREG))
		dprintf(2, "warning: cannot load non-regular file %s\n", f->path);
	else if ((f->data = malloc(f->statbuf.st_size)) == NULL)
		perror("malloc");
	else if (read(f->fd, f->data, f->statbuf.st_size) < 0)
		perror("read");
	else
		return (true);
	return (false);
}

void	file_close(t_file *f)
{
	close(f->fd);
	free(f->data);
}
