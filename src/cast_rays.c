/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cast_rays.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 15:21:40 by ljylhank          #+#    #+#             */
/*   Updated: 2025/02/05 16:18:11 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ray.h"
#include "minirt.h"
#include <math.h>
// TODO REMOVE STDIO AFTER DEBUG
#include <stdio.h>

static void	ray_to_cam_rot_pos(t_minirt *minirt, double m[3][3], t_ray *r)
{
	t_ray	new;

	new.dir.x = r->dir.x * m[0][0] + r->dir.y * m[1][0] + r->dir.z * m[2][0];
	new.dir.y = r->dir.x * m[0][1] + r->dir.y * m[1][1] + r->dir.z * m[2][1];
	new.dir.z = r->dir.x * m[0][2] + r->dir.y * m[1][2] + r->dir.z * m[2][2];
	new.start = minirt->camera_coords;
	*r = new;
}

static inline void	set_cam_rot_matrix(t_minirt *minirt)
{
	t_vec3	forward;
	t_vec3	right;
	t_vec3	up;

	forward = minirt->camera_orientation;
	right = vec3_cross((t_vec3) { 0, 1, 0 }, forward);
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

static inline t_vec3	pix_to_scrspace(t_minirt *minirt, double x, double y)
{
	t_vec3	rval;

	rval.x = (2 * ((x + 0.5) / (double) minirt->mlx->width) - 1) * minirt->aspect_ratio;
	rval.y = -(2 * ((y + 0.5) / (double) minirt->mlx->height) - 1);
	rval.z = 0;
	return (rval);
}

/*
	multiplying by 0.01745 converts degrees to radians
*/
static t_ray	create_ray(t_minirt *minirt, int x, int y)
{
	t_ray	new_ray;
	t_vec3	pos_screenspace;
	double	scr_dist_from_cmr;

	scr_dist_from_cmr = 1 / tan(minirt->camera_field_of_view / 2 * 0.01745);
	pos_screenspace = pix_to_scrspace(minirt, (double) x, (double) y);
	pos_screenspace.z = scr_dist_from_cmr;
	new_ray.dir = vec3_normalize(pos_screenspace);
	new_ray.len = 9999;
	return (new_ray);
}

void	cast_rays(t_minirt *m)
{
	t_ray	ray;
	int		column;
	int		row;
	double	dist;
	size_t	i;

	m->aspect_ratio = (double) m->mlx->width / (double) m->mlx->height;
	set_cam_rot_matrix(m);
	row = -1;
	while (++row < m->mlx->height)
	{
		column = -1;
		while (++column < m->mlx->width)
		{
			ray = create_ray(m, column, row);
			ray_to_cam_rot_pos(m, m->cam_rot_matrix, &ray);
			dist = INFINITY;
			i = (size_t) - 1;
			while (++i < m->spheres_length)
				dist = fmin(dist, sphere_intersect_dist(ray, m->spheres[i]));
			i = (size_t) - 1;
			// while (++i < m->planes_length)
			// 	dist = fmin(dist, plane_intersect_dist(ray, m->planes[i]));
			// i = (size_t) - 1;
			// while (++i < m->cylinders_length)
			// 	dist = fmin(dist, cylinder_intersect_dist(ray, m->cylinders[i]));


			m->img->pixels[4 * (row * m->mlx->width + column) + 0] = 255 / (1. + .2 * dist * dist);
			m->img->pixels[4 * (row * m->mlx->width + column) + 1] = 255 / (1. + .2 * dist * dist);
			m->img->pixels[4 * (row * m->mlx->width + column) + 2] = 255 / (1. + .2 * dist * dist);
			m->img->pixels[4 * (row * m->mlx->width + column) + 3] = 255;
		}
	}
}
