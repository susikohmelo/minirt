/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cast_rays.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 15:21:40 by ljylhank          #+#    #+#             */
/*   Updated: 2025/02/14 15:36:45 by ljylhank         ###   ########.fr       */
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
	(void)m; (void)name; (void)v;
	//if (!m->cursor_pointing)
		return ;
	//printf("%s = {%g, %g, %g} ; ", name, v.x, v.y, v.z);
}

void	mrt_print_double(t_minirt *m, const char *name, double x)
{
	(void)m; (void)name; (void)x;
	//if (!m->cursor_pointing)
		return ;
	//printf("%s = %g ; ", name, x);
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
	double	diffuse_reflection;
	const double	alpha = 10.0;
	double			shape_rough;
	t_vec3			shape_color;
	t_vec3			surface;
	t_vec3			light_dir;
	t_vec3			reflection;
	size_t			i;
	t_ray			light_ray;

	// if (ray_data.shape_type == SHAPE_PLANE)
	// 	printf("normal %g,%g,%g\n", normal.x, normal.y, normal.z);

	shape_color = ray_data.shape->color;
	shape_rough = ray_data.shape->default_rough;
	// roughness is between 0 and 1. 0 is smooth, 1 is rough
	//if (ray_data.is_reflect != INFINITY)
	//	shape_rough = ray_data.is_reflect;
	if (ray_data.shape->roughness_map)
		shape_rough = get_rough_value(ray, ray_data.shape, ray_data.shape_type);
	if (ray_data.shape->texture)
	{
		if (ray_data.is_reflect == INFINITY)
			shape_color = vec3_mul(get_albedo_blur(ray, ray_data.shape, ray_data.shape_type, 0), shape_color);
		else
			shape_color = vec3_mul(get_albedo_blur(ray, ray_data.shape, ray_data.shape_type, ray_data.is_reflect), shape_color);
	}
	surface = (t_vec3){};
	i = (size_t) - 1;
	while (++i < m->lights_length)
	{
		t_vec3 light = vec3_sub(m->lights[i].coords, ray);
		light_dir = vec3_normalize(light);
		diffuse_reflection = fmax(0, vec3_dot(light_dir, normal));
		// if (diffuse_reflection > 0)
		{
			light_ray = (t_ray){
				.start = vec3_add(ray, vec3_muls(normal, .0001)),
				.dir = light_dir,
				.length = INFINITY};
            // if (ray_data.inside_shape && ray_data.shape_type == SHAPE_SPHERE
            //     && vec3_length(vec3_sub(ray_data.shape->coords, ray_data.start)) <= ((t_sphere *)(ray_data.shape))->radius)
            //     continue ;
            // else
            if (!ray_data.inside_shape)
                get_shape_intersect_dist(m, &light_ray, ray_data.shape);
            else
				get_shape_intersect_dist(m, &light_ray, NULL);
			if (light_ray.length * light_ray.length <= vec3_dot(light, light))
                continue ;
		}
		reflection = vec3_sub( \
			vec3_muls(normal, 2 * vec3_dot(light_dir, normal)), \
			light_dir);
		surface = vec3_add(surface,
	// All I've changed here is multiply diffuse by the roughness (reducing it for smooth stuff)
	// and multiply the inverse (1 - roughness) for specular, (making it shinier for smooth stuff)
	// TODO these parameters correspond to `specular_reflection` and `diffuse_reflection`,
	// combine them somehow. Also, they will not be constants, but will be parsed for each
	// shape later on.
		vec3_add(vec3_muls(m->lights[i].color, diffuse_reflection * shape_rough), \
		vec3_muls(m->lights[i].color, \
		specular_reflection * pow(fmax(-vec3_dot(reflection, ray_data.dir), 0), alpha * (1 - shape_rough)))));
	}
	return (vec3_mul(vec3_add(m->ambient_light, surface), shape_color));
}

t_vec3	get_obj_normal(t_minirt *m, t_vec3 ray, t_ray *data)
{
	const double	normal_strength = 1;
	t_vec3			map_normal;
	t_vec3			normal;
	double			n;

	if (m->cursor_pointing) switch (data->shape_type) {
		case SHAPE_NO_SHAPE:;
		case SHAPE_SPHERE: printf("Sphere: "); break;
		case SHAPE_PLANE: printf("Plane: "); break;
		case SHAPE_CYLINDER: printf("Cylinder: "); break;
		case SHAPE_DISC: printf("Disc: "); break;
		case SHAPES_LENGTH:;
	}
	ray = vec3_add(vec3_muls(data->dir, data->length), data->start);
	if (data->shape_type == SHAPE_SPHERE)
	{
		normal = vec3_normalize(vec3_sub(ray, data->shape->coords));
		//if (vec3_dot(normal, ray) >= 0.)
		if (vec3_dot(normal, data->dir) >= 0.)
		{
			normal = vec3_muls(normal, -1);
			data->inside_shape = true;
		}
	}
	else if (data->shape_type == SHAPE_PLANE || data->shape_type == SHAPE_DISC)
	{
		normal = ((t_plane *)data->shape)->normal;
		//if (vec3_dot(normal, ray) >= 0.0001)
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
		// ((P-C)-(V*m))
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

t_vec3	surface_color(t_minirt *m, t_ray data, bool is_reflection)
{
	t_vec3			ray;
	t_vec3			cmr_dir;
	t_vec3			main_color;
	t_vec3			normal;
	double			reflect;

	ray = vec3_add(vec3_muls(data.dir, data.length), data.start);
	normal = get_obj_normal(m, ray, &data);
	if (is_reflection)
		return phong(m, ray, normal, data);
	main_color = phong(m, ray, normal, data);
	reflect = data.shape->default_rough;
	if (data.shape->roughness_map)
		reflect = get_rough_value(ray, data.shape, data.shape_type);
	data.is_reflect = reflect;
	cmr_dir = vec3_normalize(vec3_sub(m->camera_coords, ray));
	data.dir = vec3_sub(vec3_muls(normal, 2 * vec3_dot(cmr_dir, normal)), cmr_dir);
	data.start = vec3_add(ray, vec3_muls(normal, 0.001));
	data.length = INFINITY;
	get_shape_intersect_dist(m, &data, NULL);
	if (isinf(data.length) || data.length < 0.0001)
		return (main_color);
	else
	{
		normal = get_obj_normal(m, ray, &data);
		main_color = vec3_add(vec3_muls(surface_color(m, data, true), (1 - reflect) / (1 + reflect * data.length * 16)), main_color);
	}
	return (main_color);
}

/*
void	draw_scaled_pixel(t_minirt *m, t_vec3 clr, size_t col, size_t row)
{
	size_t	x;
	size_t	y;
	size_t	ind;

	ind = row * m->img->width + col;
	if (m->valid_pixel_i > 0)
	{
		m->img->pixels[4 * ind + 0] = 255 * clr.r;
		m->img->pixels[4 * ind + 1] = 255 * clr.g;
		m->img->pixels[4 * ind + 2] = 255 * clr.b;
		m->img->pixels[4 * ind + 3] = 255;
		return ;
	}
	y = -1;
	while (++y < 8)
	{
		x = -1;
		while (++x < 8 && (row + y) * m->img->width + col + x < m->img->width * m->img->height)
		{
			m->img->pixels[4 * (ind + y * m->img->width + x) + 0] = 255 * clr.r;
			m->img->pixels[4 * (ind + y * m->img->width + x) + 1] = 255 * clr.g;
			m->img->pixels[4 * (ind + y * m->img->width + x) + 2] = 255 * clr.b;
			m->img->pixels[4 * (ind + y * m->img->width + x) + 3] = 255;
		}
	}
}
*/
void	draw_scaled_pixel(t_minirt *m, t_vec3 clr, size_t col, size_t row)
{
	size_t	x;
	size_t	indx;

	indx = row * m->img->width + col;
	if (m->valid_pixel_i > 0)
	{
		m->img->pixels[4 * indx + 0] = 255 * clr.r;
		m->img->pixels[4 * indx + 1] = 255 * clr.g;
		m->img->pixels[4 * indx + 2] = 255 * clr.b;
		m->img->pixels[4 * indx + 3] = 255;
		return ;
	}
	x = -1;
	while (++x < 8 && row * m->img->width + col + x < m->img->width * m->img->height)
	{
		m->img->pixels[4 * (indx + x) + 0] = 255 * clr.r;
		m->img->pixels[4 * (indx + x) + 1] = 255 * clr.g;
		m->img->pixels[4 * (indx + x) + 2] = 255 * clr.b;
		m->img->pixels[4 * (indx + x) + 3] = 255;
	}
}
//TODO make sure window doesn't crash with under 8 pixels

static inline void	set_cursor_pointing(t_minirt *m, size_t column, size_t row)
{
	bool	cursor_in_range;

	cursor_in_range = m->mouse_x >= (int)column
		&& m->mouse_x <= (int)column + 8
		&& m->mouse_y == (int)row;
	m->cursor_pointing = !m->resizing && row != 0 && column != 0 \
		&& row < m->img->height - 10 && column != m->img->width - 10 \
		&& cursor_in_range && m->valid_pixel_i == 0;
}

void	cast_rays(t_minirt *m)
{
	t_ray	ray;
	size_t	column;
	size_t	row;
	size_t	i_pixel;
	t_vec3	color;

	printf("\r                                                                 "
		"                                                                  \r");
	m->aspect_ratio = (double) m->img->width / (double) m->img->height;
	set_cam_rot_matrix(m);

	row = (size_t) - 1;
	while (++row < m->img->height)
	{
		column = (size_t) - 1;
		while (++column < m->img->width)
		{
			set_cursor_pointing(m, column, row);
			i_pixel = row * m->img->width + column;
			if (m->valid_pixel[i_pixel & (sizeof m->valid_pixel - 1)]
				|| (i_pixel & (sizeof m->valid_pixel - 1)) != m->valid_pixel_i)
				continue;

			ray = create_ray(m, column, row);
			ray_to_cam_rot_pos(m->cam_rot_matrix, &ray);

			get_shape_intersect_dist(m, &ray, NULL);
			if (isinf(ray.length))
				color = (t_vec3){};
			else
			{
				color = surface_color(m, ray, false);
				mrt_print(color);
				color.r = fmin(fmax(color.r, 0), 1);
				color.g = fmin(fmax(color.g, 0), 1);
				color.b = fmin(fmax(color.b, 0), 1);
			}
			draw_scaled_pixel(m, color, column, row);
		}
	}
	fflush(stdout); // TODO get rid of this!
}
