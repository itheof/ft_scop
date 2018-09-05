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

uniform bool	is_selected;
uniform float	texture_ratio;

uniform sampler2D _texture0;
uniform sampler2D _texture1;

void main()
{
	vec4	combined;
	vec4	base_color = vec4(0.3529, 0.3236, 0.3236, 1.0);

	if (texture(_texture1, TexCoord) != vec4(1.0, 1.0, 1.0, 1.0))
		combined = texture(_texture1, TexCoord);
	else
		combined = texture(_texture0, TexCoord);
	combined = mix(base_color, combined, texture_ratio);
	if (is_selected)
		FragColor = mix(combined, vec4(1.0, 0.0, 0.0, 0.5), 0.2);
	else
		FragColor = combined;
}
