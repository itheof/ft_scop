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
layout (location = 1) in vec2 uv_mapping;

uniform mat4 	model;
uniform mat4 	view;
uniform mat4 	projection;

out vec2		TexCoord;

void main()
{
  gl_Position = projection * view * model * vec4(aPos, 1.0);
	TexCoord = uv_mapping;
}
