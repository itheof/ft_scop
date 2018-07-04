#ifndef SHADER_H
# define SHADER_H

# include "libft/stdbool.h"
# include "file.h"
# include <glad/glad.h>

# define SHADERS_DIR ASSETS_DIR "shaders/"

typedef	struct	s_shader
{
	unsigned 	id;
	GLenum		type;
	char const	*path;
}				t_shader;

t_bool		shader_init(t_shader *shader);
void		shader_deinit(t_shader *shader);

#endif