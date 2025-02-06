/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cast_rays.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 15:21:40 by ljylhank          #+#    #+#             */
/*   Updated: 2025/02/06 20:26:04 by ljylhank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ray.h"
#include "minirt.h"
#include <math.h>




#include <stdio.h>




void	mrt_print_vec3(t_minirt *m, const char *name, t_vec3 v)
{
	if (!m->cursor_pointing)
		return ;
	printf("%s = {%g, %g, %g} ; ", name, v.x, v.y, v.z);
}

void	mrt_print_double(t_minirt *m, const char *name, double x)
{
	if (!m->cursor_pointing)
		return ;
	printf("%s = %g ; ", name, x);
}




static void	ray_to_cam_rot_pos(t_minirt *minirt, double m[3][3], t_ray *r)
{
	r->dir.x = r->dir.x * m[0][0] + r->dir.y * m[1][0] + r->dir.z * m[2][0];
	r->dir.y = r->dir.x * m[0][1] + r->dir.y * m[1][1] + r->dir.z * m[2][1];
	r->dir.z = r->dir.x * m[0][2] + r->dir.y * m[1][2] + r->dir.z * m[2][2];
	r->start = minirt->camera_coords;
}

static inline void	set_cam_rot_matrix(t_minirt *minirt)
{
	t_vec3	forward;
	t_vec3	right;
	t_vec3	up;

	forward = minirt->camera_orientation;
	right = vec3_cross(vec3(0, 1, 0 ), forward);
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
	new_ray.length = INFINITY;
	return (new_ray);
}

static t_vec3	phong(
	t_minirt *m, t_vec3 ray, t_vec3 normal, const t_shape *shape)
{
	const double	specular_reflection = 1.5;
	const double	diffuse_reflection = 0.75;
	const double	alpha = 10.0;
	t_vec3			shape_color;
	t_vec3			surface;
	t_vec3			light;
	t_vec3			reflection;
	size_t			i;

	// TODO atm this assumes the shape is a sphere
	if (shape->texture)
		shape_color = get_texture_color(ray, shape, SHAPE_SPHERE);
	else
		shape_color = shape->color;

	surface = (t_vec3){};
	i = (size_t) - 1;
	while (++i < 1)
	{
		light = vec3_normalize(vec3_sub(m->light_coords, ray));
		ray = vec3_normalize(ray); // TODO don't recalculate!
		reflection = vec3_sub( \
			vec3_muls(normal, 2 * vec3_dot(light, normal)), \
			light);
		surface = vec3_add(surface, vec3_add(vec3_muls(m->light_color, diffuse_reflection * fmax(vec3_dot(light, normal), 0)), vec3_muls(m->light_color, specular_reflection * pow(fmax(-vec3_dot(reflection, ray), 0), alpha))));
	}
	return (vec3_mul(vec3_add(m->ambient_light, surface), shape_color));
}

t_vec3	surface_color(t_minirt *m, t_ray data)
{
	t_vec3	ray;
	t_vec3	normal;

	ray = vec3_add(vec3_muls(data.dir, data.length), data.start);
	if (data.shape_type == SHAPE_SPHERE)
		normal = vec3_normalize(vec3_sub(ray, ((t_sphere*)data.shape)->coords));
	else if (data.shape_type == SHAPE_PLANE)
		normal = (t_vec3){};
	else if (data.shape_type == SHAPE_CYLINDER)
		normal = (t_vec3){};
	else
		return (t_vec3){};
	return phong(m, ray, normal, data.shape);
}

void	cast_rays(t_minirt *m)
{
	t_ray	ray;
	int32_t	column;
	int32_t	row;
	size_t	i;
	t_vec3	color;

	printf("\r                                                                 "
		"                                                                  \r");
	m->aspect_ratio = (double) m->mlx->width / (double) m->mlx->height;
	set_cam_rot_matrix(m);

	row = -1;
	while (++row < m->mlx->height)
	{
		column = -1;
		while (++column < m->mlx->width)
		{
			m->cursor_pointing = m->mouse_x == row && m->mouse_y == column;
			ray = create_ray(m, column, row);
			ray_to_cam_rot_pos(m, m->cam_rot_matrix, &ray);
			i = (size_t) - 1;
			while (++i < m->spheres_length)
				min_sphere_intersect_dist(&ray, &m->spheres[i]);
			// i = (size_t) - 1;
			// while (++i < m->planes_length)
			// 	dist = fmin(dist, plane_intersect_dist(ray, m->planes[i]));
			// i = (size_t) - 1;
			// while (++i < m->cylinders_length)
			// 	dist = fmin(dist, cylinder_intersect_dist(ray, m->cylinders[i]));

			// double temp = ray.dir.x;
			// ray.dir.x = -ray.dir.y;
			// ray.dir.y = -temp;

			color = surface_color(m, ray);
			mrt_print(color);
			color.r = fmin(fmax(color.r, 0), 1);
			color.g = fmin(fmax(color.g, 0), 1);
			color.b = fmin(fmax(color.b, 0), 1);
			m->img->pixels[4 * (row * m->mlx->width + column) + 0] = 255 * color.r;
			m->img->pixels[4 * (row * m->mlx->width + column) + 1] = 255 * color.g;
			m->img->pixels[4 * (row * m->mlx->width + column) + 2] = 255 * color.b;
			m->img->pixels[4 * (row * m->mlx->width + column) + 3] = 255;

			// m->img->pixels[4 * (row * m->mlx->width + column) + 0] = 255 / (1. + .2 * ray.length * ray.length);
			// m->img->pixels[4 * (row * m->mlx->width + column) + 1] = 255 / (1. + .2 * ray.length * ray.length);
			// m->img->pixels[4 * (row * m->mlx->width + column) + 2] = 255 / (1. + .2 * ray.length * ray.length);
			// m->img->pixels[4 * (row * m->mlx->width + column) + 3] = 255;
		}
	}
	fflush(stdout); // TODO get rid of this!
}
