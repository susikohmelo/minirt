/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_object_normal.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 20:42:31 by ljylhank          #+#    #+#             */
/*   Updated: 2025/02/19 15:45:47 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ray.h"
#include "minirt.h"

static t_vec3	cylinder_normal(t_vec3 ray, t_ray *data)
{
	t_vec3	c;
	t_vec3	axis;
	double	m;
	t_vec3	x;

	axis = ((t_cylinder *)data->shape)->axis;
	c = vec3_add(\
		((t_cylinder *)data->shape)->coords, \
		vec3_muls(\
			axis, \
			((t_cylinder *)data->shape)->height / 2));
	x = vec3_sub(data->start, c);
	m = vec3_dot(data->dir, axis) * data->length + vec3_dot(x, axis);
	return (vec3_normalize(\
		vec3_sub(\
			vec3_sub(\
				ray, \
				c), \
			vec3_muls(\
				axis, \
				m))));
}

t_vec3	get_obj_normal(t_vec3 ray, t_ray *data)
{
	t_vec3			map_normal;
	t_vec3			normal;

	normal = (t_vec3){};
	ray = vec3_add(vec3_muls(data->dir, data->length), data->start);
	if (data->shape_type == SHAPE_SPHERE)
		normal = vec3_normalize(vec3_sub(ray, data->shape->coords));
	else if (data->shape_type == SHAPE_PLANE || data->shape_type == SHAPE_DISC)
		normal = ((t_plane *)data->shape)->normal;
	else if (data->shape_type == SHAPE_CYLINDER)
		normal = cylinder_normal(ray, data);
	if (vec3_dot(normal, data->dir) >= 0.)
	{
		normal = vec3_muls(normal, -1);
		data->inside_shape = true;
	}
	if (!data->shape->normal_map)
		return (normal);
	map_normal = vec3_inverse_lookat(get_texture_color(\
		ray, NORMAL_MAP, data->shape, data->shape_type), normal);
	return (vec3_normalize(map_normal));
}
