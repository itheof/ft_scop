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
in vec3		ourColor;
in vec2		TexCoord;


uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	if (texture(texture2, TexCoord) != vec4(1.0, 1.0, 1.0, 1.0))
		FragColor = texture(texture2, TexCoord);
	else
		FragColor = mix(texture(texture1, TexCoord), vec4(ourColor, 1.0), 0.3);
}
