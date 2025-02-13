/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_uv_coords.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 17:51:21 by ljylhank          #+#    #+#             */
/*   Updated: 2025/02/12 21:12:04 by ljylhank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>

static double	wrap_coords(double coord)
{
	coord = coord - (int) coord;
	if (coord < 0)
		coord = 1 + coord;
	return (coord);
}

t_vec2	get_plane_uv(t_vec3 intersect, t_plane *plane, int shape)
{
	t_vec3	local_intersect;
	t_vec2	uv;
	t_disc	*disc;

	if (shape == SHAPE_PLANE)
	{
		local_intersect = vec3_sub(intersect, plane->coords);
		local_intersect = vec3_lookat(local_intersect, plane->normal);
		uv.x = wrap_coords(local_intersect.x);
		uv.y = wrap_coords(local_intersect.y);
	}
	else
	{
		disc = (t_disc *) plane;
		local_intersect = vec3_sub(intersect, disc->coords);
		local_intersect = vec3_lookat(local_intersect, disc->normal);
		uv.x = wrap_coords(local_intersect.x / (M_PI * disc->radius * 2));
		uv.y = wrap_coords(local_intersect.y / (M_PI * disc->radius * 2));
	}
	return (uv);
}

t_vec2	get_cylinder_uv(t_vec3 intersect, t_cylinder *cylinder)
{
	t_vec3	local_intersect;
	t_vec2	uv;

	local_intersect = vec3_sub(intersect, cylinder->coords);
	local_intersect = vec3_lookat(local_intersect, cylinder->axis);
	uv.x = atan2(local_intersect.y, local_intersect.x) / TWO_PI + 0.5;
	uv.y = wrap_coords(local_intersect.z / (M_PI * cylinder->radius * 2));
	return (uv);
}

t_vec2	get_sphere_uv(t_vec3 intersect, t_sphere *sphere)
{
	t_vec3	local_intersect;
	t_vec2	uv;

	local_intersect = vec3_normalize(vec3_sub(intersect, sphere->coords));
	uv.x = atan2(local_intersect.x, local_intersect.z) / TWO_PI + 0.5;
	uv.y = local_intersect.y * 0.5 + 0.5;
	return (uv);
}
