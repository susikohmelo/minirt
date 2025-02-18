/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   surface_get_colors.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 20:44:23 by ljylhank          #+#    #+#             */
/*   Updated: 2025/02/18 20:46:41 by ljylhank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ray.h"
#include "minirt.h"
#include <math.h>

double	get_shape_roughness(t_ray *data, t_vec3 *ray)
{
	if (data->shape->roughness_map)
		return (get_rough_value(*ray, data->shape, data->shape_type));
	else
		return (data->shape->default_rough);
}

t_vec3	get_shape_color(t_ray *data, t_vec3 *ray)
{
	if (data->shape->texture)
		return (vec3_mul(get_albedo_blur(*ray,
					data->shape, data->shape_type, 0), data->shape->color));
	else
		return (data->shape->color);
}

t_vec3	skybox_color(t_minirt *m, t_ray data, t_vec3 ray, double roughness)
{
	t_vec3			shape_color;
	t_vec3			skybox_color;
	t_vec3			skybox_diffuse;

	shape_color = get_shape_color(&data, &ray);
	skybox_color = get_skybox_color(m, data.dir, roughness);
	skybox_diffuse = vec3_mul(skybox_color, shape_color);
	skybox_color = vec3_add(vec3_muls(skybox_diffuse, roughness),
			vec3_muls(skybox_color, 1 - roughness));
	skybox_color = vec3_sub(skybox_color,
			vec3_mul(m->ambient_light, shape_color));
	skybox_color = vec3_clamp(skybox_color, 0, 1);
	return (skybox_color);
}
