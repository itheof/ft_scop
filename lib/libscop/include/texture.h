#ifndef TEXTURE_H
# define TEXTURE_H

# include "libft/stdbool.h"

typedef struct	s_texture
{
	unsigned		id;
	char const		*path;
	unsigned char	*data;
	int				width;
	int				height;
}				t_texture;

t_bool		texture_init(t_texture *t);
void		texture_deinit(t_texture *t);

#endif
