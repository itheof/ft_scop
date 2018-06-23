/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_scop.vs                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/18 09:12:21 by tvallee           #+#    #+#             */
/*   Updated: 2018/06/18 09:12:44 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3		ourColor;
uniform vec2	off;

void main()
{
    gl_Position = vec4(aPos.x + off.x, aPos.y + off.y, aPos.z, 1.0);
	ourColor = aColor;
}
