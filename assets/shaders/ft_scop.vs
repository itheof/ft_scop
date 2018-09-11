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
layout (location = 2) in float color;

uniform mat4 	_model;
uniform mat4 	_view;
uniform mat4 	_projection;
uniform vec3	_model_offset;
uniform float	_model_scale;

out vec2		TexCoord;
out float		Color;

void main()
{
	Color = color;
	gl_Position = _projection * _view * _model * vec4((aPos + _model_offset) * _model_scale, 1.0);
	TexCoord = uv_mapping;
}
