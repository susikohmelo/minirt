/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   surface_color.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 20:38:19 by ljylhank          #+#    #+#             */
/*   Updated: 2025/02/18 20:44:16 by ljylhank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ray.h"
#include "minirt.h"
#include <math.h>

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
