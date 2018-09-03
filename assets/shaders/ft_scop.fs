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

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
	if (texture(texture1, TexCoord) != vec4(1.0, 1.0, 1.0, 1.0))
		FragColor = texture(texture1, TexCoord);
	else
		FragColor = texture(texture0, TexCoord);
}
