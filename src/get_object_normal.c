/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_object_normal.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 20:42:31 by ljylhank          #+#    #+#             */
/*   Updated: 2025/02/18 20:42:47 by ljylhank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ray.h"
#include "minirt.h"
#include <math.h>

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
