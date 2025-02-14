/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_frame.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 21:06:44 by ljylhank          #+#    #+#             */
/*   Updated: 2025/02/14 12:40:05 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

#define SCREEN_STRING_SIZE 2048 // TODO move this to header

char	*sphere_string(char *buf)
{
	ft_strlcat(buf, "Sphere         X", SCREEN_STRING_SIZE);
	return (buf);
}

char	*plane_string(char *buf)
{
	ft_strlcat(buf, "Plane          X", SCREEN_STRING_SIZE);
	return (buf);
}

char	*cylinder_string(char *buf)
{
	ft_strlcat(buf, "Cylinder       X", SCREEN_STRING_SIZE);
	return (buf);
}

char	*disc_string(char *buf)
{
	ft_strlcat(buf, "Cylinder       X", SCREEN_STRING_SIZE);
	return (buf);
}

char	*default_string(char *buf)
{
	ft_strlcat(buf, "o", SCREEN_STRING_SIZE);
	return (buf);
}

static char	*screen_string(t_minirt *m)
{
	static char	buf[SCREEN_STRING_SIZE];

	ft_memset(buf, '\0', sizeof buf);
	if (m->shape_type == SHAPE_SPHERE)
		return (sphere_string(buf));
	else if (m->shape_type == SHAPE_PLANE)
		return (plane_string(buf));
	else if (m->shape_type == SHAPE_CYLINDER)
		return (cylinder_string(buf));
	else if (m->shape_type == SHAPE_DISC)
		return (disc_string(buf));
	return (default_string(buf));
}

void	render_frame(void *minirt)
{
	static mlx_image_t	*text;

	const char	ones[] = "\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1";
	t_minirt	*m;

	m = minirt;
	mlx_delete_image(m->mlx, text);
	cast_rays(minirt);
	m->valid_pixel[m->valid_pixel_i] = true;
	m->valid_pixel_i = (m->valid_pixel_i + 5) & (sizeof m->valid_pixel - 1);
	if (ft_memcmp(m->valid_pixel, ones, sizeof m->valid_pixel) == 0)
	{
		m->double_clicked = false;
		m->resizing = false;
	}
	text = mlx_put_string(m->mlx, screen_string(m), 0, 0);
}
