/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_texture_color.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 21:11:33 by ljylhank          #+#    #+#             */
/*   Updated: 2025/02/15 21:59:01 by ljylhank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>

t_vec3	get_albedo_blur(t_vec3 intersect, const t_shape *shape,
			int shape_type, double blur)
{
	t_vec2		uv;
	mlx_image_t	*img;
	t_vec3		vect;

	img = NULL;
	uv = (t_vec2){};
	if (shape_type == SHAPE_SPHERE)
		uv = get_sphere_uv(intersect, (t_sphere *) shape);
	else if (shape_type == SHAPE_PLANE || shape_type == SHAPE_DISC)
		uv = get_plane_uv(intersect, (t_plane *) shape, shape_type);
	else if (shape_type == SHAPE_CYLINDER)
		uv = get_cylinder_uv(intersect, (t_cylinder *) shape);
	if (uv.x > 1. || uv.x < 0. || uv.y > 1. || uv.y < 0.)
		return (vec3(0, 0, 0));
	img = shape->texture;
	vect = get_texture_from_uv(img, uv.x, uv.y, blur / (0.15 + blur) * 1.15);
	return (vect);
}

static inline void	flip_normal_coords(t_vec3 *vect)
{
	vect->x = -vect->x * 2 + 1;
	vect->y = vect->y * 2 - 1;
	vect->z = vect->z * 2 - 1;
}

t_vec3	get_texture_color(t_vec3 intersect, int texture_type,
			const t_shape *shape, int shape_type)
{
	t_vec2		uv;
	mlx_image_t	*img;
	t_vec3		vect;

	img = NULL;
	uv = (t_vec2){};
	if (shape_type == SHAPE_SPHERE)
		uv = get_sphere_uv(intersect, (t_sphere *) shape);
	else if (shape_type == SHAPE_PLANE || shape_type == SHAPE_DISC)
		uv = get_plane_uv(intersect, (t_plane *) shape, shape_type);
	else if (shape_type == SHAPE_CYLINDER)
		uv = get_cylinder_uv(intersect, (t_cylinder *) shape);
	if (uv.x > 1. || uv.x < 0. || uv.y > 1. || uv.y < 0.)
		return (vec3(0, 0, 0));
	if (texture_type == ALBEDO)
		img = shape->texture;
	else if (texture_type == NORMAL_MAP)
		img = shape->normal_map;
	vect = get_texture_from_uv(img, uv.x, uv.y, 0);
	if (texture_type != NORMAL_MAP)
		return (vect);
	flip_normal_coords(&vect);
	return (vect);
}

double	get_rough_value(t_vec3 intersect, const t_shape *shape, int shape_type)
{
	t_vec2		uv;
	mlx_image_t	*img;
	t_vec3		vect;

	img = NULL;
	uv = (t_vec2){};
	if (shape_type == SHAPE_SPHERE)
		uv = get_sphere_uv(intersect, (t_sphere *) shape);
	else if (shape_type == SHAPE_PLANE || shape_type == SHAPE_DISC)
		uv = get_plane_uv(intersect, (t_plane *) shape, shape_type);
	else if (shape_type == SHAPE_CYLINDER)
		uv = get_cylinder_uv(intersect, (t_cylinder *) shape);
	if (uv.x > 1. || uv.x < 0. || uv.y > 1. || uv.y < 0.)
		return (shape->default_rough);
	img = shape->roughness_map;
	vect = get_texture_from_uv(img, uv.x, uv.y, 0);
	return ((vect.r + vect.g + vect.b) / 3);
}
