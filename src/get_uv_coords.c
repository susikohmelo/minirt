/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_uv_coords.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 17:51:21 by ljylhank          #+#    #+#             */
/*   Updated: 2025/02/06 18:14:26 by ljylhank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>


static t_vec2	get_sphere_uv(t_vec3 r, t_vec3 s_pos)
{
	t_vec3	intersect;
	t_vec2	uv;

	intersect = vec3_normalize(vec3_sub(r, s_pos));
	uv.x = atan2(intersect.x, intersect.z) / TWO_PI + 0.5;
	uv.y = intersect.y * 0.5 + 0.5;
	return (uv);
}

t_vec3	get_texture_color(t_vec3 r, const t_shape *shape, int shape_type)
{
	t_vec2	uv;

	if (shape_type == SHAPE_SPHERE)
		uv = get_sphere_uv(r, shape->coords);
	return(get_texture_from_uv(shape->texture, uv.x, uv.y));
}
