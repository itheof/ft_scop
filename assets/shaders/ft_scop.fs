/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_scop.fs                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/18 09:12:21 by tvallee           #+#    #+#             */
/*   Updated: 2018/06/18 09:12:44 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#version 330 core

out vec4	FragColor;
in vec2		TexCoord;
in float	Color;

uniform bool	is_selected;
uniform float	texture_ratio;

uniform sampler2D _texture0;

void main()
{
	vec4	combined;
	vec4	base_color = vec4(Color, 0.4, 0.8, 1.0);

	combined = mix(base_color, texture(_texture0, TexCoord), texture_ratio);
	if (is_selected)
		FragColor = mix(combined, vec4(1.0, 0.0, 0.0, 0.5), 0.2);
	else
		FragColor = combined;
}
