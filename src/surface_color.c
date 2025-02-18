/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   surface_color.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 20:38:19 by ljylhank          #+#    #+#             */
/*   Updated: 2025/02/18 22:09:14 by ljylhank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ray.h"
#include "minirt.h"
#include <math.h>

static inline void	shoot_reflection(t_minirt *m, t_ray *data, double *rough)
{
	t_vec3			normal;
	t_vec3			intersect;
	t_vec3			view_dir;

	intersect = vec3_add(vec3_muls(data->dir, data->length), data->start);
	normal = get_obj_normal(m, intersect, data);
	*rough = fmin(*rough + get_shape_roughness(data, &intersect), 1);
	data->start = vec3_add(intersect, vec3_muls(normal, 0.001));
	view_dir = vec3_muls(data->dir, -1);
	data->dir = vec3_sub(vec3_muls(\
				normal, 2 * vec3_dot(view_dir, normal)), view_dir);
	data->length = INFINITY;
	get_shape_intersect_dist(m, data, NULL);
}

static inline bool	reflect(t_minirt *m, t_vec3 *main_color, t_ray *data,
							double *roughness)
{
	t_vec3			intersect;
	t_vec3			shape_color;
	t_vec3			shape_difus;
	double			new_rough;

	shoot_reflection(m, data, roughness);
	if (!isinf(data->length))
	{
		intersect = vec3_add(vec3_muls(data->dir, data->length), data->start);
		new_rough = get_shape_roughness(data, &intersect);
		data->is_reflect = new_rough;
		shape_color = vec3_muls(surface_color(m, *data, true),
				(1 - new_rough) / (1 + new_rough * data->length * 16));
		shape_difus = vec3_mul(shape_color, get_shape_color(data, &intersect));
		shape_color = vec3_add(vec3_muls(shape_color, 1 - new_rough),
				vec3_muls(shape_difus, new_rough));
		*main_color = vec3_add(*main_color,
				vec3_muls(shape_color, 1 - *roughness));
		return (true);
	}
	return (false);
}

static inline t_vec3	reflections_reflections(t_minirt *m, t_vec3 main_color,
						t_ray *data, double roughness)
{
	t_vec3			shape_color;
	t_vec3			shape_difus;
	t_vec3			intersect;
	int				i;

	main_color = vec3_add(main_color, vec3_muls(surface_color(m, *data, true),
				(1 - roughness) / (1 + roughness * data->length * 16)));
	i = 0;
	while (++i < m->max_ray_bounces && roughness < 1)
	{
		if (reflect(m, &main_color, data, &roughness))
			continue ;
		intersect = vec3_add(vec3_muls(data->dir, data->length), data->start);
		shape_color = skybox_color(m, *data, intersect, roughness);
		shape_difus = vec3_mul(shape_color, get_shape_color(data, &intersect));
		shape_color = vec3_add(vec3_muls(shape_color, 1 - roughness),
				vec3_muls(shape_difus, roughness));
		return (vec3_add(main_color, shape_color));
	}
	return (main_color);
}

t_vec3	surface_color(t_minirt *m, t_ray data, bool is_reflection)
{
	t_vec3			main_color;
	t_vec3			intersect;
	t_vec3			view_dir;
	t_vec3			normal;
	double			roughness;

	intersect = vec3_add(vec3_muls(data.dir, data.length), data.start);
	normal = get_obj_normal(m, intersect, &data);
	main_color = phong(m, intersect, normal, data);
	if (is_reflection || m->max_ray_bounces == 0)
		return (main_color);
	roughness = get_shape_roughness(&data, &intersect);
	data.is_reflect = roughness;
	view_dir = vec3_normalize(vec3_sub(m->camera_coords, intersect));
	data.dir = vec3_sub(vec3_muls(\
				normal, 2 * vec3_dot(view_dir, normal)), view_dir);
	data.start = vec3_add(intersect, vec3_muls(normal, 0.001));
	data.length = INFINITY;
	get_shape_intersect_dist(m, &data, NULL);
	if (isinf(data.length) || data.length < 0.0001)
		return (vec3_add(main_color,
				skybox_color(m, data, intersect, roughness)));
	else
		main_color = reflections_reflections(m, main_color, &data, roughness);
	return (main_color);
}
