/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cast_rays.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 15:21:40 by ljylhank          #+#    #+#             */
/*   Updated: 2025/02/11 17:35:37 by ljylhank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ray.h"
#include "minirt.h"
#include <math.h>




#include <stdio.h>

// Does nothing, works just as a placeholder to but breakpoints into.
void	mrt_break(void)
{
	(void)0;
}

void	mrt_debug(t_minirt *mrt)
{
	if (mrt->cursor_pointing)
		mrt_break();
}

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
	t_vec3	tmp;

	tmp = r->dir;
	r->dir.x = tmp.x * m[0][0] + tmp.y * m[1][0] + tmp.z * m[2][0];
	r->dir.y = tmp.x * m[0][1] + tmp.y * m[1][1] + tmp.z * m[2][1];
	r->dir.z = tmp.x * m[0][2] + tmp.y * m[1][2] + tmp.z * m[2][2];
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
	new_ray.is_reflect = 0;
	return (new_ray);
}

static t_vec3	phong(
	t_minirt *m, t_vec3 ray, t_vec3 normal, t_ray ray_data)
{
	const double	specular_reflection = 4;
	const double	diffuse_reflection = 6;
	const double	alpha = 10.0;
	double			shape_rough;
	t_vec3			shape_color;
	t_vec3			surface;
	t_vec3			light;
	t_vec3			reflection;
	size_t			i;
	t_ray			light_ray;

	shape_color = ray_data.shape->color;
	shape_rough = 0.5;
	// roughness is between 0 and 1. 0 is smooth, 1 is rough
	if (ray_data.is_reflect > 0)
		shape_rough = ray_data.is_reflect;
	else if (ray_data.shape->roughness_map)
		shape_rough = get_rough_value(ray, ray_data.shape, ray_data.shape_type);
	if (ray_data.shape->texture)
		shape_color = vec3_mul(get_albedo_blur(ray, ray_data.shape, ray_data.shape_type, shape_rough * (ray_data.is_reflect > 0)), shape_color);

	surface = (t_vec3){};
	i = (size_t) - 1; // TODO when casting to light sources, skip all the ones
	while (++i < m->lights_length)   // behind the objects (normal dot light_direction >= 0)
	{
		light_ray = (t_ray){
			.start = ray,
			.dir = vec3_normalize(vec3_sub(m->lights[i].coords, ray)),
			.length = INFINITY};
		get_shape_intersect_dist(m, &light_ray, ray_data.shape);
		if (!isinf(light_ray.length))
			continue ;

		light = vec3_normalize(vec3_sub(m->lights[i].coords, ray));
		reflection = vec3_sub( \
			vec3_muls(normal, 2 * vec3_dot(light, normal)), \
			light);
		surface = vec3_add(surface,
	// All I've changed here is multiply diffuse by the roughness (reducing it for smooth stuff)
	// and multiply the inverse (1 - roughness) for specular, (making it shinier for smooth stuff)
	// TODO these parameters correspond to `specular_reflection` and `diffuse_reflection`,
	// combine them somehow. Also, they will not be constants, but will be parsed for each
	// shape later on.
		vec3_add(vec3_muls(m->lights[i].color, diffuse_reflection * shape_rough * \
		fmax(vec3_dot(light, normal), 0)), vec3_muls(m->lights[i].color, (1 - shape_rough) * \
		specular_reflection * pow(fmax(-vec3_dot(reflection, ray_data.dir), 0), alpha))));
	}
	return (vec3_mul(vec3_add(m->ambient_light, surface), shape_color));
}

t_vec3	get_obj_normal(t_vec3 ray, t_ray data)
{
	const double	normal_strength = 0.5;
	t_vec3			map_normal;
	t_vec3			normal;
	double			n;

	if (data.shape_type == SHAPE_SPHERE)
		normal = vec3_normalize(vec3_sub(ray, data.shape->coords));
	else if (data.shape_type == SHAPE_PLANE)
	{
		normal = ((t_plane *)data.shape)->normal;
		if (vec3_dot(normal, ray) >= 0.)
			normal = vec3_muls(normal, -1);
	}
	else if (data.shape_type == SHAPE_CYLINDER)
	{
		n = vec3_dot(ray, ((t_cylinder *)data.shape)->axis) \
			+ vec3_dot(vec3_sub(data.start, data.shape->coords), \
				((t_cylinder *)data.shape)->axis);
		normal = vec3_normalize(vec3_sub(vec3_sub( \
			ray, data.shape->coords), vec3_muls(
				((t_cylinder *)data.shape)->axis, n)));
	}
	else // should there be some logic for lighting here? And is this dead code?
		return (t_vec3){};
	if (data.shape->normal_map)
	{
		map_normal = vec3_inverse_lookat(get_texture_color( \
			ray, NORMAL_MAP, data.shape, data.shape_type), normal);
		normal = vec3_normalize(vec3_add(vec3_muls(normal, 1 - normal_strength), \
		vec3_muls(map_normal, normal_strength)));
	}
	return (normal);
}

t_vec3	surface_color(t_minirt *m, t_ray data, bool is_reflection)
{
	t_vec3			ray;
	t_vec3			cmr_dir;
	t_vec3			main_color;
	t_vec3			normal;
	double			reflect;

	ray = vec3_add(vec3_muls(data.dir, data.length), data.start);
	normal = get_obj_normal(ray, data);
	if (is_reflection)
		return phong(m, ray, normal, data);
	main_color = phong(m, ray, normal, data);
	reflect = 0.5;
	if (data.shape->roughness_map)
		reflect = get_rough_value(ray, data.shape, data.shape_type);
	data.is_reflect = 0.5;
	if (data.shape->roughness_map)
		data.is_reflect = reflect;
	reflect = pow(1 - reflect / (0.5 + reflect) * 1.5, 2);
	cmr_dir = vec3_normalize(vec3_sub(m->camera_coords, ray));
	data.dir = vec3_sub(vec3_muls(normal, 2 * vec3_dot(cmr_dir, normal)), cmr_dir);
	data.start = vec3_add(ray, vec3_muls(data.dir, 0.001));
	data.length = INFINITY;
	get_shape_intersect_dist(m, &data, NULL);
	if (isinf(data.length) || data.length < 0.0001)
		return (main_color);
	else
	{
		normal = get_obj_normal(ray, data);
		main_color = vec3_add(vec3_muls(surface_color(m, data, true), 1 / sqrt(data.length + 1) * reflect), main_color);
		mrt_print(main_color);
		main_color.r = fmin(fmax(main_color.r, 0), 1);
		main_color.g = fmin(fmax(main_color.g, 0), 1);
		main_color.b = fmin(fmax(main_color.b, 0), 1);
	}
	return (main_color);
}

void	cast_rays(t_minirt *m)
{
	t_ray	ray;
	int32_t	column;
	int32_t	row;
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
			if (row != 0 && column != 0)
				m->cursor_pointing = m->mouse_x == row && m->mouse_y == column;
			ray = create_ray(m, column, row);
			ray_to_cam_rot_pos(m, m->cam_rot_matrix, &ray);

			get_shape_intersect_dist(m, &ray, NULL);
			if (isinf(ray.length))
			{
				color = (t_vec3){};
				mrt_print(color);
			}
			else
			{
				color = surface_color(m, ray, false);
				mrt_print(color);
				color.r = fmin(fmax(color.r, 0), 1);
				color.g = fmin(fmax(color.g, 0), 1);
				color.b = fmin(fmax(color.b, 0), 1);
			}
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
