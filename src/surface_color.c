/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   surface_color.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 20:38:19 by ljylhank          #+#    #+#             */
/*   Updated: 2025/02/19 16:43:21 by ljylhank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ray.h"
#include "minirt.h"
#include <math.h>

static inline t_vec3	mix_dif_reflect(t_ray *data, t_vec3 shape_reflect,
							t_vec3 *intersect, double rough)
{
	t_vec3			shape_difus;

	shape_difus = vec3_mul(shape_reflect, get_shape_color(data, intersect));
	return (vec3_add(vec3_muls(shape_reflect, 1 - rough),
			vec3_muls(shape_difus, rough)));
}

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
							double *rough)
{
	t_vec3			intersect;
	t_vec3			shape_color;
	double			new_rough;

	shoot_reflection(m, data, rough);
	if (!isinf(data->length))
	{
		intersect = vec3_add(vec3_muls(data->dir, data->length), data->start);
		new_rough = get_shape_roughness(data, &intersect);
		data->is_reflect = new_rough;
		shape_color = vec3_muls(surface_color(m, *data, true),
				(1 - new_rough) / (1 + new_rough * data->length * 16));
		shape_color = mix_dif_reflect(data, shape_color, &intersect, *rough);
		*main_color = vec3_add(*main_color,
				vec3_muls(shape_color, 1 - *rough));
		return (true);
	}
	return (false);
}

static inline t_vec3	reflections_reflections(t_minirt *m, t_vec3 main_color,
						t_ray *data, double roughness)
{
	t_vec3			shape_color;
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
		shape_color = mix_dif_reflect(data, shape_color, &intersect, roughness);
		return (vec3_add(main_color, shape_color));
	}
	return (main_color);
}

t_vec3	surface_color(t_minirt *m, t_ray data, bool is_reflection)
{
	t_vec3			main_clr;
	t_vec3			intersect;
	t_vec3			view_dir;
	t_vec3			normal;
	double			rough;

	intersect = vec3_add(vec3_muls(data.dir, data.length), data.start);
	normal = get_obj_normal(m, intersect, &data);
	main_clr = phong(m, intersect, normal, data);
	if (is_reflection || m->max_ray_bounces == 0)
		return (main_clr);
	rough = get_shape_roughness(&data, &intersect);
	data.is_reflect = rough;
	view_dir = vec3_normalize(vec3_sub(m->camera_coords, intersect));
	data.dir = vec3_sub(vec3_muls(\
				normal, 2 * vec3_dot(view_dir, normal)), view_dir);
	data.start = vec3_add(intersect, vec3_muls(normal, 0.001));
	data.length = INFINITY;
	get_shape_intersect_dist(m, &data, NULL);
	if (isinf(data.length) || data.length < 0.0001)
		return (vec3_add(main_clr, mix_dif_reflect(&data, skybox_color(\
							m, data, intersect, rough), &intersect, rough)));
	return (reflections_reflections(m, main_clr, &data, rough));
}
