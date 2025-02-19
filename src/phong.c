/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phong.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 11:49:38 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/19 21:57:50 by ljylhank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>

static void	get_material_attributes(
	t_vec3 *shape_color, double *shape_rough, t_vec3 ray, t_ray data)
{
	*shape_color = data.shape->color;
	*shape_rough = data.shape->roughness;
	if (data.shape->roughness_map)
		get_rough_value(ray, data.shape, data.shape_type);
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

static bool	surface_to_light(t_minirt *m, t_ray *light_ray, t_ray data)
{
	t_vec3	light;

	light = light_ray->dir;
	light_ray->dir = vec3_normalize(light);
	if (!data.inside_shape)
		get_shape_intersect_dist(m, light_ray, data.shape);
	else
		get_shape_intersect_dist(m, light_ray, NULL);
	return (!(light_ray->length * light_ray->length <= vec3_dot(light, light)));
}

// t_plane just happens to have appropriate data
static void	add_light_contribution(
	t_vec3 *surface, t_vec3 *surface_speculars, t_plane light, t_ray data)
{
	double			diffusion;
	t_vec3			reflection;
	t_vec3			light_ray_dir;

	light_ray_dir = light.coords;
	diffusion = fmax(0, vec3_dot(light_ray_dir, light.normal));
	reflection = vec3_sub(\
		vec3_muls(light.normal, 2 * vec3_dot(light_ray_dir, light.normal)), \
		light_ray_dir);
	*surface = vec3_add(*surface, vec3_muls(light.color, diffusion));
	*surface_speculars = vec3_add(*surface_speculars, \
		vec3_muls(light.color, (1 / pow(light.roughness + 0.88, 2) - 0.27) * \
		pow(\
			fmax(-vec3_dot(reflection, data.dir), 0), \
			(1 / pow(light.roughness + 0.01, 2) + 0.02))));
}

t_vec3	phong(
	t_minirt *m, t_vec3 ray, t_vec3 normal, t_ray data)
{
	t_shape			shape;
	t_vec3			surface;
	t_vec3			surface_speculars;
	size_t			i;
	t_ray			light_ray;

	shape = (t_shape){};
	get_material_attributes(&shape.color, &shape.roughness, ray, data);
	surface = (t_vec3){};
	surface_speculars = (t_vec3){};
	i = (size_t) - 1;
	while (++i < m->lights_length)
	{
		light_ray = (t_ray){
			.start = vec3_add(ray, vec3_muls(normal, .0001)),
			.dir = vec3_sub(m->lights[i].coords, ray), .length = INFINITY};
		if (!surface_to_light(m, &light_ray, data))
			continue ;
		add_light_contribution(&surface, &surface_speculars, (t_plane){
			light_ray.dir, m->lights[i].color,
			.roughness = shape.roughness, normal}, data);
	}
	return (vec3_add(\
		vec3_mul(vec3_add(m->ambient_light, surface), shape.color), \
		surface_speculars));
}
