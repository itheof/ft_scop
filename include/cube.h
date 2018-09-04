#ifndef CUBE_H
# define CUBE_H
# include "vector.h"
# include "libscop.h"

typedef struct	s_cube
{
	t_object	obj;
	t_vector	move;
	char		uid;
}				t_cube;

extern t_object const	g_cube_obj;

#endif
