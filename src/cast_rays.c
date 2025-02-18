/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cast_rays.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 15:21:40 by ljylhank          #+#    #+#             */
/*   Updated: 2025/02/18 11:02:14 by lfiestas         ###   ########.fr       */
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
	printf("\r                                                                 "
		"                                                                  \r");
	printf("%s = {%g, %g, %g} ; ", name, v.x, v.y, v.z);
}

void	mrt_print_double(t_minirt *m, const char *name, double x)
{
	if (!m->cursor_pointing)
		return ;
	printf("\r                                                                 "
		"                                                                  \r");
	printf("%s = %g ; ", name, x);
}

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

static t_vec3	phong(
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

t_vec3	get_obj_normal(t_minirt *m, t_vec3 ray, t_ray *data)
{
	const double	normal_strength = 1;
	t_vec3			map_normal;
	t_vec3			normal;
	double			n;

	(void)m; // TODO get rid of this
	ray = vec3_add(vec3_muls(data->dir, data->length), data->start);
	if (data->shape_type == SHAPE_SPHERE)
	{
		normal = vec3_normalize(vec3_sub(ray, data->shape->coords));
		if (vec3_dot(normal, data->dir) >= 0.)
		{
			normal = vec3_muls(normal, -1);
			data->inside_shape = true;
		}
	}
	else if (data->shape_type == SHAPE_PLANE || data->shape_type == SHAPE_DISC)
	{
		normal = ((t_plane *)data->shape)->normal;
		if (vec3_dot(normal, data->dir) >= 0.0001)
        {
			normal = vec3_muls(normal, -1);
            data->inside_shape = true;
        }
	}
	else if (data->shape_type == SHAPE_CYLINDER)
	{
		t_vec3	C = vec3_add(
			((t_cylinder *)data->shape)->coords,
			vec3_muls(
				((t_cylinder *)data->shape)->axis,
				((t_cylinder *)data->shape)->height / 2));

		t_vec3	P = ray;
		t_vec3	D = data->dir;
		t_vec3	V = ((t_cylinder *)data->shape)->axis;
		t_vec3	X = vec3_sub(data->start, C);

		double	t = data->length;
		double	m = vec3_dot(D,V) * t + vec3_dot(X, V);
		normal = vec3_normalize(
			vec3_sub(
				vec3_sub(
					P,
					C),
				vec3_muls(
					V,
					m))); (void)n;

		if (vec3_dot(normal, data->dir) >= 0.)
		{
			normal = vec3_muls(normal, -1);
			data->inside_shape = true;
		}
	}
	if (data->shape->normal_map)
	{
		map_normal = vec3_inverse_lookat(get_texture_color( \
			ray, NORMAL_MAP, data->shape, data->shape_type), normal);
		normal = vec3_normalize(vec3_add(vec3_muls(normal, 1 - normal_strength), \
			vec3_muls(map_normal, normal_strength)));
	}
	return (normal);
}

static inline double	get_shape_roughness(t_ray *data, t_vec3 *ray)
{
	if (data->shape->roughness_map)
		return (get_rough_value(*ray, data->shape, data->shape_type));
	else
		return (data->shape->default_rough);
}

static inline t_vec3	get_shape_color(t_ray *data, t_vec3 *ray)
{
	if (data->shape->texture)
		return (vec3_mul(get_albedo_blur(*ray,
			data->shape, data->shape_type, 0), data->shape->color));
	else
		return (data->shape->color);
}

static inline t_vec3	skybox_color(t_minirt *m, t_ray data,
							t_vec3 ray, double roughness)
{
	t_vec3			shape_color;
	t_vec3			skybox_color;
	t_vec3			skybox_diffuse;

	shape_color = get_shape_color(&data, &ray);
	skybox_color = get_skybox_color(m, data.dir, roughness);
	skybox_diffuse = vec3_mul(skybox_color, shape_color);
	skybox_color = vec3_add(vec3_muls(skybox_diffuse, roughness), vec3_muls(skybox_color, 1 - roughness));
	skybox_color = vec3_sub(skybox_color, vec3_mul(m->ambient_light, shape_color));

	//TODO function for this, the same thing is used in cast_rays() too
	skybox_color.r = fmin(fmax(skybox_color.r, 0), 1);
	skybox_color.g = fmin(fmax(skybox_color.g, 0), 1);
	skybox_color.b = fmin(fmax(skybox_color.b, 0), 1);
	return (skybox_color);
}

t_vec3	surface_color(t_minirt *m, t_ray data, bool is_reflection)
{
	t_vec3			ray;
	t_vec3			view_dir;
	t_vec3			main_color;
	t_vec3			shape_color;
	t_vec3			shape_diffuse;
	t_vec3			normal;
	double			roughness;
	double			temp_rough;
	int				i;

	ray = vec3_add(vec3_muls(data.dir, data.length), data.start);
	normal = get_obj_normal(m, ray, &data);
	main_color = phong(m, ray, normal, data);

	// If we are being called through recursion, kill the cycle here
	// Or we don't want reflections at all
	if (is_reflection || m->max_ray_bounces == 0)
		return (main_color);
	roughness = get_shape_roughness(&data, &ray);
	data.is_reflect = roughness;

	// Set new ray to point to the reflection direction and check collision
	view_dir = vec3_normalize(vec3_sub(m->camera_coords, ray));
	data.dir = vec3_sub(vec3_muls(normal, 2 * vec3_dot(view_dir, normal)), view_dir);
	data.start = vec3_add(ray, vec3_muls(normal, 0.001));
	data.length = INFINITY;
	get_shape_intersect_dist(m, &data, NULL);

	// If we hit nothing, just get the skybox color and return
	if (isinf(data.length) || data.length < 0.0001)
		return (vec3_add(main_color, skybox_color(m, data, ray, roughness)));
	else
	{
		// Get color of the object we hit via recursion
		main_color = vec3_add(main_color, vec3_muls(surface_color(m, data, true),
			(1 - roughness) / (1 + roughness * data.length * 16)));

		// This is a loop for any further reflections of reflections of reflections etc...
		// Loop is killed if the light ray hits nothing, max bounces are reached or roughness is 1
		i = 0;
		while (++i < m->max_ray_bounces && roughness < 1)
		{
			// Set ray to point to new reflected direction from the object we hit last time
			normal = get_obj_normal(m, ray, &data);
			ray = vec3_add(vec3_muls(data.dir, data.length), data.start);
			roughness = fmin(roughness + get_shape_roughness(&data, &ray), 1);
			data.start = vec3_add(ray, vec3_muls(normal, 0.001));
			view_dir = vec3_muls(data.dir, -1);
			data.dir = vec3_sub(vec3_muls(normal, 2 * vec3_dot(view_dir, normal)), view_dir);
			data.length = INFINITY;
			get_shape_intersect_dist(m, &data, NULL);

			// If we hit something, add the new color via recursion,
			// continue the cycle to check for more reflections
			if (!isinf(data.length))
			{
				ray = vec3_add(vec3_muls(data.dir, data.length), data.start);
				temp_rough = get_shape_roughness(&data, &ray);
				data.is_reflect = temp_rough;

				//TODO This mixing of specular / difuse can definitely be put into it's
				// own function, it is used 2 or 3 times
				shape_color = vec3_muls(surface_color(m, data, true),
					(1 - temp_rough) / (1 + temp_rough * data.length * 16));
				shape_diffuse = vec3_mul(shape_color, get_shape_color(&data, &ray));
				shape_color = vec3_add(vec3_muls(shape_color, 1 - temp_rough),
					vec3_muls(shape_diffuse, temp_rough));
				main_color = vec3_add(main_color, vec3_muls(shape_color, 1 - roughness));
				continue ;
			}

			// If we do not hit an object, kill the loop and get the skybox color
			// Mix color with roughness/color of the object we last hit
			ray = vec3_add(vec3_muls(data.dir, data.length), data.start);
			shape_color = skybox_color(m, data, ray, roughness);
			shape_diffuse = vec3_mul(shape_color, get_shape_color(&data, &ray));
			shape_color = vec3_add(vec3_muls(shape_color, 1 - roughness),
				vec3_muls(shape_diffuse, roughness));
			return (vec3_add(main_color, shape_color));
		}
	}
	return (main_color);
}

void	draw_scaled_pixel(t_minirt *m, t_vec3 clr, size_t col, size_t row)
{
	size_t	x;
	size_t	y;
	size_t	idx;
	size_t	const_lens[3];
	int		offset;

	idx = row * m->img->width + col;
	if (m->valid_pixel_y > 0 || m->valid_pixel_x > 0)
	{
		m->img->pixels[4 * idx + 0] = 255 * clr.r;
		m->img->pixels[4 * idx + 1] = 255 * clr.g;
		m->img->pixels[4 * idx + 2] = 255 * clr.b;
		m->img->pixels[4 * idx + 3] = 255;
		return ;
	}
	const_lens[0] = m->img->width * m->img->height;
	y = (size_t) - 1;
	while (++y <= m->valid_pixel_len)
	{
		offset = -1 * m->valid_pixel_len * (y % 2 != 0);
		const_lens[1] = (row + y) * m->img->width;
		const_lens[2] = 4 * (idx + y * m->img->width);
		x = (size_t) - 1;
		while (++x <= m->valid_pixel_len && const_lens[1] + col + x + offset
				< const_lens[0] && col + x + offset < m->img->width)
		{
			m->img->pixels[const_lens[2] + (x + offset) * 4 + 0] = 255 * clr.r;
			m->img->pixels[const_lens[2] + (x + offset) * 4 + 1] = 255 * clr.g;
			m->img->pixels[const_lens[2] + (x + offset) * 4 + 2] = 255 * clr.b;
			m->img->pixels[const_lens[2] + (x + offset) * 4 + 3] = 255;
		}
	}
}

//TODO make sure window doesn't crash with under 32 pixels
//TODO We don't actually need this, just cast a single ray in hooks!
static inline void	set_cursor_pointing(t_minirt *m, size_t column, size_t row)
{
	bool	cursor_in_range;

	column = fmax(column - (double)m->valid_pixel_len / 2, 0);
	row = fmax(row - (double)m->valid_pixel_len / 2, 0);
	cursor_in_range = \
		m->mouse_x >= (int) column
		&& (size_t) m->mouse_x <= column + m->valid_pixel_len
		&& (size_t) m->mouse_y >= row
		&& (size_t) m->mouse_y <= row + m->valid_pixel_len;
	m->cursor_pointing = !m->resizing && row != 0 && column != 0 \
		&& row < m->img->height - 10 && column != m->img->width - 10 \
		&& cursor_in_range && m->valid_pixel_x == 0;
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

void	cast_rays(t_minirt *m)
{
	t_ray	ray;
	size_t	column;
	size_t	row;
	size_t	i_pixel[2];
	t_vec3	color;

	precalculate(m);
	set_cam_rot_matrix(m);

	row = (size_t) - 1;
	i_pixel[0] = (size_t) - 1;
	while (++row < m->img->height)
	{
		i_pixel[0] = (i_pixel[0] + 1) * (i_pixel[0] < m->valid_pixel_len);
		if (m->valid_pixel_y == 0 && i_pixel[0] != m->valid_pixel_y)
			continue ;
		i_pixel[1] = (size_t) - 1;
		column = (size_t) - 1;
		while (++column < m->img->width)
		{
			i_pixel[1] = (i_pixel[1] + 1) * (i_pixel[1] < m->valid_pixel_len);
			if (i_pixel[1] != m->valid_pixel_x)
				continue ;
			set_cursor_pointing(m, column, row);
			ray = cast_ray(m, column, row);

			if (isinf(ray.length))
				color = get_skybox_color(m, ray.dir, 0);
			else if (ray.shape_type != SHAPE_LIGHT)
			{
				color = surface_color(m, ray, false);
				color.r = fmin(fmax(color.r, 0), 1);
				color.g = fmin(fmax(color.g, 0), 1);
				color.b = fmin(fmax(color.b, 0), 1);
			}
			else
				color = ((t_light *)ray.shape)->color_value;
			draw_scaled_pixel(m, color, column, row);
		}
	}
	fflush(stdout); // TODO get rid of this!
}
