#ifndef CUBE_H
# define CUBE_H
# include "vector.h"
# include "libscop.h"

typedef struct			s_cube
{
	t_object		obj; /* NOTE: assumed to be first member */
	struct s_cube	*next;
	struct s_cube	*prev;
	t_vector		move;
	char			uid;
	float			texture_ratio;
	float			texture_change_axis;
	t_bool			texture_toggled;
	t_bool			rotating;
	void			(*unfocus)(void *obj);
}						t_cube;

extern t_object const	g_cube_obj;

#endif
