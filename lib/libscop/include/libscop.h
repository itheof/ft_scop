#ifndef LIBSCOP_H
# define LIBSCOP_H

# include "vector.h"
# include "matrix.h"
# include "file.h"
# include "program.h"
# include "angle_conv.h"
# include "texture.h"
# include "display.h"
# include "camera.h"
# include "object.h"

t_bool	libscop_init(void (* (*get_proc_addr)(const char *))(void));
void	libscop_deinit(void);

#endif
