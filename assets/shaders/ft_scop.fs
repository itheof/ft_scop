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
in vec3		color;

/*
uniform sampler2D texture1;
uniform sampler2D texture2;
*/

void main()
{
	FragColor = vec4(color, 1.0);
	/*if (texture(texture2, TexCoord) != vec4(1.0, 1.0, 1.0, 1.0))
		FragColor = texture(texture2, TexCoord);
	else
		FragColor = texture(texture1, TexCoord);*/
}
