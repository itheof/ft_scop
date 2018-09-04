/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libscop.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/04 14:17:39 by tvallee           #+#    #+#             */
/*   Updated: 2018/09/04 14:17:41 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

t_bool	libscop_init(void (*(*get_proc_addr)(const char *))(void));
void	libscop_deinit(void);

#endif
