/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wfobj.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/07 11:07:38 by tvallee           #+#    #+#             */
/*   Updated: 2018/09/07 12:16:01 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WFOBJ_H
# define WFOBJ_H

#include "libft.h"

typedef struct	s_wfobj
{
	t_list	*v;
	t_list	*vt;
	t_list	*vn;
	t_list	*vp;
/*	t_list	*cstype;
	t_list	*deg;
	t_list	*bmat;
	t_list	*step; */
	t_list	*p;
	t_list	*l;
	t_list	*f;
	t_list	*curv;
	t_list	*curv2;
	t_list	*surf;
/*	t_list	*parm;
	t_list	*trim;
	t_list	*hole;
	t_list	*scrv;
	t_list	*sp;
	t_list	*end;
	t_list	*con;*/
}				t_wfobj;

t_wfobj	*wfobj_parse(FILE *stream);
void	wfobj_delete(t_wfobj *obj);

#endif
