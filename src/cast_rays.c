/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cast_rays.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 15:21:40 by ljylhank          #+#    #+#             */
/*   Updated: 2025/02/18 23:26:46 by ljylhank         ###   ########.fr       */
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
		right = vec3_normalize(vec3_cross(vec3(0, 1, 0 ), forward));
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

t_vec3	phong(
	t_minirt *m, t_vec3 ray, t_vec3 normal, t_ray ray_data)
{
	const double	specular_reflection = 1;
	double			diffuse_reflection;
	const double	alpha = 1.0;
	double			shape_rough;
	t_vec3			shape_color;
	t_vec3			surface;
	t_vec3			surface_speculars;
	t_vec3			light_dir;
	t_vec3			reflection;
	size_t			i;
	t_ray			light_ray;

	shape_color = ray_data.shape->color;
	shape_rough = ray_data.shape->default_rough;
	// roughness is between 0 and 1. 0 is smooth, 1 is rough
	//if (ray_data.is_reflect != INFINITY)
	//	shape_rough = ray_data.is_reflect;
	if (ray_data.shape->roughness_map)
		// shape_rough = get_rough_value(ray, ray_data.shape, ray_data.shape_type);
		shape_rough = fmax(0, 2 * ray_data.shape->default_rough - 1) \
			+ (1 - fabs(2 * ray_data.shape->default_rough - 1)) \
				* get_rough_value(ray, ray_data.shape, ray_data.shape_type);
	if (ray_data.shape->texture)
	{
		if (ray_data.is_reflect == INFINITY)
			shape_color = vec3_mul(get_albedo_blur(ray, ray_data.shape, ray_data.shape_type, 0), shape_color);
		else
			shape_color = vec3_mul(get_albedo_blur(ray, ray_data.shape, ray_data.shape_type, ray_data.is_reflect), shape_color);
	}
	surface = (t_vec3){};
	surface_speculars = (t_vec3){};
	i = (size_t) - 1;
	while (++i < m->lights_length)
	{
		t_vec3 light = vec3_sub(m->lights[i].coords, ray);
		light_dir = vec3_normalize(light);
		diffuse_reflection = fmax(0, vec3_dot(light_dir, normal));

		light_ray = (t_ray){
			.start = vec3_add(ray, vec3_muls(normal, .0001)),
			.dir = light_dir,
			.length = INFINITY};
		if (!ray_data.inside_shape)
			get_shape_intersect_dist(m, &light_ray, ray_data.shape);
		else
			get_shape_intersect_dist(m, &light_ray, NULL);
		if (light_ray.length * light_ray.length <= vec3_dot(light, light))
			continue ;

		reflection = vec3_sub( \
			vec3_muls(normal, 2 * vec3_dot(light_dir, normal)), \
			light_dir);
		surface = vec3_add(surface, vec3_muls(m->lights[i].color, diffuse_reflection));
		surface_speculars = vec3_add(surface_speculars, \
			vec3_muls(m->lights[i].color, specular_reflection * (1 / pow(shape_rough + 0.88, 2) - 0.27) * \
			pow(fmax(-vec3_dot(reflection, ray_data.dir), 0), alpha * (1 / pow(shape_rough + 0.01, 2) + 0.02))));
	}
	return (vec3_add(vec3_mul(vec3_add(m->ambient_light, surface), shape_color), surface_speculars));
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

static void min_light_intersect_dist(t_ray *ray, const t_light *light)
{
	t_vec3	lstart;
	double	b;
	double	c;
	double	discriminant;
	double	length;

	lstart = vec3_sub(ray->start, light->coords);
	b = 2 * vec3_dot(ray->dir, lstart);
	c = vec3_dot(lstart, lstart) - (light->brightness + .5);
	discriminant = b * b - 4 * 1 * c;
	if (discriminant >= 0)
	{
		length = (-b - sqrt(discriminant)) / (2. * 1);
		if (length < ray->length && length >= 0)
		{
			ray->length = length;
			ray->shape = (t_shape *)light;
			ray->shape_type = SHAPE_LIGHT;
		}
	}
}

void	get_light_intersect_dist(t_minirt *m, t_ray *ray)
{
	size_t	i;

	i = (size_t) - 1;
	while (++i < m->lights_length)
		min_light_intersect_dist(ray, &m->lights[i]);
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

void	cast_rays(t_minirt *m, size_t tid)
{
	t_ray	ray;
	size_t	column;
	size_t	row;
	size_t	i_pixel;
	t_vec3	color;

	precalculate(m);
	set_cam_rot_matrix(m);

	row = (size_t) - 1;
	while (++row < m->img->height)
	{
		#if !THREADS
		(void)tid;
		#else
		if ((row & (THREADS - 1)) != tid)
		 	continue ;
		#endif
		column = (size_t) - 1;
		while (++column < m->img->width)
		{
			i_pixel = row * m->img->width + column;
			if (m->valid_pixel[i_pixel & (m->valid_pixel_len - 1)]
				|| (i_pixel & (m->valid_pixel_len - 1)) != m->valid_pixel_i)
				continue;
			ray = cast_ray(m, column, row);

			if (isinf(ray.length))
				color = get_skybox_color(m, ray.dir, 0);
			else if (ray.shape_type != SHAPE_LIGHT)
				color = vec3_clamp(surface_color(m, ray, false), 0, 1);
			else
				color = ((t_light *)ray.shape)->color_value;
			draw_scaled_pixel(m, color, column, row);
		}
	}
}
