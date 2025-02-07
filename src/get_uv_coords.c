/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_uv_coords.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 17:51:21 by ljylhank          #+#    #+#             */
/*   Updated: 2025/02/07 04:27:44 by ljylhank         ###   ########.fr       */
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

t_vec3	get_texture_color(t_vec3 r, int	texture_type,
			const t_shape *shape, int shape_type)
{
	t_vec2		uv;
	mlx_image_t	*img;
	t_vec3		vect;

	img = NULL;
	if (shape_type == SHAPE_SPHERE)
		uv = get_sphere_uv(r, shape->coords);
	if (texture_type == ALBEDO)
		img = shape->texture;
	else if (texture_type == NORMAL_MAP)
		img = shape->normal_map;
	else if (texture_type == ROUGHNESS_MAP)
		img = shape->roughness_map;
	vect = get_texture_from_uv(img, uv.x, uv.y);
	if (texture_type == NORMAL_MAP)
	{
		vect.x = -vect.x * 2 + 1;
		vect.y = vect.y * 2 - 1;
		vect.z = vect.z * 2 - 1;
	}
	return(vect);
}

double	get_rough_value(t_vec3 r, int	texture_type,
			const t_shape *shape, int shape_type)
{
	t_vec2		uv;
	mlx_image_t	*img;
	t_vec3		vect;

	img = NULL;
	if (shape_type == SHAPE_SPHERE)
		uv = get_sphere_uv(r, shape->coords);
	if (texture_type == ALBEDO)
		img = shape->texture;
	else if (texture_type == NORMAL_MAP)
		img = shape->normal_map;
	else if (texture_type == ROUGHNESS_MAP)
		img = shape->roughness_map;
	vect = get_texture_from_uv(img, uv.x, uv.y);
	return((vect.r + vect.g + vect.b) / 3);
}
