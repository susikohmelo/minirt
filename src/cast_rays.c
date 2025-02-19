/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cast_rays.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 15:21:40 by ljylhank          #+#    #+#             */
/*   Updated: 2025/02/19 13:47:11 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ray.h"
#include "minirt.h"
#include <math.h>

static void	set_cam_rot_matrix(t_minirt *minirt)
{
	t_vec3	forward;
	t_vec3	right;
	t_vec3	up;

	forward = minirt->camera_orientation;
	if (forward.y == 1 || forward.y == -1)
		right = vec3(1, 0, 0);
	else
		right = vec3_normalize(vec3_cross(vec3(0, 1, 0), forward));
	up = vec3_cross(forward, right);
	minirt->cam_rot_matrix[0][0] = right.x;
	minirt->cam_rot_matrix[0][1] = right.y;
	minirt->cam_rot_matrix[0][2] = right.z;
	minirt->cam_rot_matrix[1][0] = up.x;
	minirt->cam_rot_matrix[1][1] = up.y;
	minirt->cam_rot_matrix[1][2] = up.z;
	minirt->cam_rot_matrix[2][0] = forward.x;
	minirt->cam_rot_matrix[2][1] = forward.y;
	minirt->cam_rot_matrix[2][2] = forward.z;
}

static void	precalculate(t_minirt *m)
{
	size_t	i;

	m->aspect_ratio = (double) m->img->width / (double) m->img->height;
	m->ambient_light = vec3_muls(\
		m->ambient_light_color, m->ambient_light_ratio);
	i = (size_t) - 1;
	while (i < m->lights_length)
		m->lights[i].color = vec3_muls(\
			m->lights[i].color_value, m->lights[i].brightness);
}

static void	draw_scaled_pixel(t_minirt *m, t_vec3 clr, size_t col, size_t row)
{
	size_t	i;
	size_t	x;
	size_t	ix;

	clr = vec3_muls(clr, 255);
	i = row * m->img->width + col;
	x = (size_t) - 1;
	while (++x < m->valid_pixel_len
		&& !m->valid_pixel[(m->valid_pixel_i + x) & (m->valid_pixel_len - 1)]
		&& col + x < m->img->width)
	{
		ix = (i + x) * 4;
		m->img->pixels[ix + 0] = clr.r;
		m->img->pixels[ix + 1] = clr.g;
		m->img->pixels[ix + 2] = clr.b;
		m->img->pixels[ix + 3] = 255;
	}
}

static t_vec3	ray_color(t_minirt *m, size_t column, size_t row)
{
	t_ray	ray;

	ray = cast_ray(m, column, row);
	if (isinf(ray.length))
		return (get_skybox_color(m, ray.dir, 0));
	else if (ray.shape_type != SHAPE_LIGHT)
		return (vec3_clamp(surface_color(m, ray, false), 0, 1));
	return (((t_light *)ray.shape)->color_value);
}

void	cast_rays(t_minirt *m, size_t tid)
{
	size_t	column;
	size_t	row;
	size_t	i_pixel;

	precalculate(m);
	set_cam_rot_matrix(m);
	row = (size_t) - 1;
	while (++row < m->img->height)
	{
		if ((row & (THREADS - 1)) != tid)
			continue ;
		column = (size_t) - 1;
		while (++column < m->img->width)
		{
			i_pixel = row * m->img->width + column;
			if (m->valid_pixel[i_pixel & (m->valid_pixel_len - 1)]
				|| (i_pixel & (m->valid_pixel_len - 1)) != m->valid_pixel_i)
				continue ;
			draw_scaled_pixel(m, ray_color(m, column, row), column, row);
		}
	}
}
