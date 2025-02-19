/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cast_rays.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 15:21:40 by ljylhank          #+#    #+#             */
/*   Updated: 2025/02/19 11:49:51 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ray.h"
#include "minirt.h"
#include <math.h>

static void	ray_to_cam_rot_pos(double m[3][3], t_ray *r)
{
	t_vec3	tmp;

	tmp = r->dir;
	r->dir.x = tmp.x * m[0][0] + tmp.y * m[1][0] + tmp.z * m[2][0];
	r->dir.y = tmp.x * m[0][1] + tmp.y * m[1][1] + tmp.z * m[2][1];
	r->dir.z = tmp.x * m[0][2] + tmp.y * m[1][2] + tmp.z * m[2][2];
}

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

static t_vec3	pix_to_scrspace(t_minirt *m, double x, double y)
{
	return ((t_vec3){
		.x = (2 * ((x + 0.5) / (double) m->img->width) - 1) * m->aspect_ratio,
		.y = -(2 * ((y + 0.5) / (double) m->img->height) - 1),
		.z = 0,
	});
}

/*
	multiplying by 0.01745 converts degrees to radians
*/
static t_ray	create_ray(t_minirt *minirt, int32_t x, int32_t y)
{
	t_ray	new_ray;
	t_vec3	pos_screenspace;
	double	scr_dist_from_cmr;

	new_ray = (t_ray){};
	scr_dist_from_cmr = 1 / tan(minirt->camera_field_of_view / 2 * 0.01745);
	pos_screenspace = pix_to_scrspace(minirt, (double) x, (double) y);
	pos_screenspace.z = scr_dist_from_cmr;
	new_ray.dir = vec3_normalize(pos_screenspace);
	new_ray.start = minirt->camera_coords;
	new_ray.length = INFINITY;
	new_ray.is_reflect = INFINITY;
	return (new_ray);
}

static inline void	draw_scaled_pixel(t_minirt *m, t_vec3 clr, size_t col, size_t row)
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

static void	precalculate(t_minirt *m)
{
	size_t	i;

	m->aspect_ratio = (double) m->img->width / (double) m->img->height;
	m->ambient_light = vec3_muls(m->ambient_light_color, m->ambient_light_ratio);
	i = (size_t) - 1;
	while (i < m->lights_length)
		m->lights[i].color = vec3_muls( \
			m->lights[i].color_value, m->lights[i].brightness);
}

t_ray	cast_ray(t_minirt *m, size_t column, size_t row)
{
	t_ray	ray;

	ray = create_ray(m, column, row);
	ray_to_cam_rot_pos(m->cam_rot_matrix, &ray);

	get_shape_intersect_dist(m, &ray, NULL);
	if (m->show_lights)
		get_light_intersect_dist(m, &ray);
	return (ray);
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
