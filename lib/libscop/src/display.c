#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include "libft/stdbool.h"
#include "display.h"
#include "matrix.h"
#include "angle_conv.h"

t_matrix		*g_projection = NULL;

void	display_toggle_wireframe(void)
{
	static t_bool	wireframe = false;

	wireframe = !wireframe;
	if (wireframe)
	{
		fprintf(stderr, "wireframe on\n");
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		fprintf(stderr, "wireframe off\n");
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void	display_set_viewport(int width, int height)
{
	free(g_projection);
	g_projection = matrix_new_perspective(ft_radian(45.0f),
		(float)width / (float)height, 0.1f, 100.0f);
	glViewport(0, 0, width, height);
}
