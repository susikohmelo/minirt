/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phong.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 11:49:38 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/19 12:07:03 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>

static void	get_material(
	t_vec3 *shape_color, double *shape_rough, t_vec3 ray, t_ray data)
{
	*shape_color = data.shape->color;
	*shape_rough = data.shape->default_rough;
	if (data.shape->roughness_map)
		*shape_rough = fmax(0, 2 * data.shape->default_rough - 1) \
			+ (1 - fabs(2 * data.shape->default_rough - 1)) \
				* get_rough_value(ray, data.shape, data.shape_type);
	if (data.shape->texture)
	{
		if (data.is_reflect == INFINITY)
			*shape_color = vec3_mul(\
				get_albedo_blur(ray, data.shape, data.shape_type, 0), \
				*shape_color);
		else
			*shape_color = vec3_mul(\
				get_albedo_blur(\
					ray, data.shape, data.shape_type, data.is_reflect), \
				*shape_color);
	}
}

t_vec3	phong(
	t_minirt *m, t_vec3 ray, t_vec3 normal, t_ray data)
{
	double			diffusion;
	double			shape_rough;
	t_vec3			shape_color;
	t_vec3			surface;
	t_vec3			surface_speculars;
	t_vec3			light_dir;
	t_vec3			reflection;
	size_t			i;
	t_ray			light_ray;
	t_vec3			light;

	get_material(&shape_color, &shape_rough, ray, data);
	surface = (t_vec3){};
	surface_speculars = (t_vec3){};
	i = (size_t) - 1;
	while (++i < m->lights_length)
	{
		light = vec3_sub(m->lights[i].coords, ray);
		light_dir = vec3_normalize(light);
		diffusion = fmax(0, vec3_dot(light_dir, normal));

		light_ray = (t_ray){
			.start = vec3_add(ray, vec3_muls(normal, .0001)),
			.dir = light_dir,
			.length = INFINITY};
		if (!data.inside_shape)
			get_shape_intersect_dist(m, &light_ray, data.shape);
		else
			get_shape_intersect_dist(m, &light_ray, NULL);
		if (light_ray.length * light_ray.length <= vec3_dot(light, light))
			continue ;

		reflection = vec3_sub( \
			vec3_muls(normal, 2 * vec3_dot(light_dir, normal)), \
			light_dir);
		surface = vec3_add(surface, vec3_muls(m->lights[i].color, diffusion));
		surface_speculars = vec3_add(surface_speculars, \
			vec3_muls(m->lights[i].color, (1 / pow(shape_rough + 0.88, 2) - 0.27) * \
			pow(fmax(-vec3_dot(reflection, data.dir), 0), (1 / pow(shape_rough + 0.01, 2) + 0.02))));
	}
	return (vec3_add(vec3_mul(vec3_add(m->ambient_light, surface), shape_color), surface_speculars));
}

