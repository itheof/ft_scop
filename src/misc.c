#include "ft_scop.h"

float	ft_degree(float rad)
{
	return (rad * 180.0f / (float)M_PI);
}

float	ft_radian(float degree)
{
	return (degree * (float)M_PI / 180.0f);
}
