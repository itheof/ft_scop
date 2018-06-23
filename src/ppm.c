/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ppm.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/23 12:40:07 by tvallee           #+#    #+#             */
/*   Updated: 2018/06/23 14:09:13 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_scop.h"
#include "libft/libc.h"

char const	*ppm_error_msg = NULL;

static char	const	*check_header(char const *data)
{
	char const	*match;

	if (!(match = ft_strchr(data, '\n')) || ft_strncmp(data, "P6", match - data))
	{
		ppm_error_msg = "bad header file for texture";
		return (NULL);
	}
	return (match + 1);
}

static char const	*skip_comments(char const *data)
{
	while (true)
	{
		if (*data != '#')
			return (data);
		if (!(data = ft_strchr(data, '\n')))
			return (NULL);
		data++;
	}
}

static char const	*get_wh(char const *current, int *width, int *height, int *maxdepth)
{
	*width = ft_atoi(current);
	while (ft_isdigit(*current))
		current++;
	while (ft_isspace(*current))
		current++;

	*height = ft_atoi(current);
	while (ft_isdigit(*current))
		current++;
	while (ft_isspace(*current))
		current++;

	*maxdepth = ft_atoi(current);
	while (ft_isdigit(*current))
		current++;
	while (ft_isspace(*current))
		current++;
	if (!*width || !*height || *maxdepth > 255)
		return (NULL);
	return (current);
}

static unsigned char	*load_data(t_file *f, char const *current, int width, int height)
{
	unsigned char	*ret;

	if (width * height * (unsigned long long)3 > f->statbuf.st_size - (size_t)(current - f->statbuf.st_size))
		return (NULL);
	if (!(ret = malloc(3 * width * height)))
		return (NULL);
	ft_memcpy(ret, current, 3 * width * height);
	return (ret);
}

unsigned char	*ppm_load(char const *path, int *width, int *height)
{
	t_file			f;
	char const		*current;
	unsigned char	*ret;
	int				maxdepth;

	ret = NULL;
	if (!file_open(&f, path, O_RDONLY) || !file_load(&f))
	{
		ppm_error_msg = "no such file or directory";
		return (NULL);
	}
	else if (!(current = check_header(f.data)))
		ppm_error_msg = "bad header";
	else if (!(current = skip_comments(current)))
		ppm_error_msg = "bad comments";
	else if (!(current = get_wh(current, width, height, &maxdepth)))
		ppm_error_msg = "bad width/height/maxdepth";
	else if (!(ret = load_data(&f, current, *width, *height)))
		ppm_error_msg = "failed to load raster data";
	file_close(&f);
	return (ret);
}
