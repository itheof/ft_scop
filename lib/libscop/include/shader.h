/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/04 16:29:52 by tvallee           #+#    #+#             */
/*   Updated: 2018/09/04 16:30:17 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHADER_H
# define SHADER_H

# include "libft/stdbool.h"
# include "file.h"

# define SHADERS_DIR ASSETS_DIR "shaders/"

typedef	struct	s_shader
{
	unsigned		id;
	unsigned int	type;
	char const		*path;
}				t_shader;

t_bool			shader_init(t_shader *shader);
void			shader_deinit(t_shader *shader);

#endif
