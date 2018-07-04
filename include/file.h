#ifndef FILE_H
# define FILE_H

# include <unistd.h>
# include <sys/stat.h>
# include <sys/types.h>
# include "libft/stdbool.h"

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

#endif
