/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_dist.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 12:29:56 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/18 18:27:27 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "ray.h"
#include <math.h>

void	min_sphere_intersect_dist(t_ray *ray, const t_sphere *sphere)
{
	t_vec3	lstart;
	double	b;
	double	c;
	double	discriminant;
	double	length;

	lstart = vec3_sub(ray->start, sphere->coords);
	b = 2 * vec3_dot(ray->dir, lstart);
	c = vec3_dot(lstart, lstart) - sphere->radius * sphere->radius;
	discriminant = b * b - 4 * 1 * c;
	if (discriminant >= 0)
	{
		length = (-b - sqrt(discriminant)) / (2. * 1);
		if (vec3_dot(lstart, lstart) <= sphere->radius * sphere->radius)
			length = (-b + sqrt(discriminant)) / (2. * 1);
		if (length < ray->length && length >= 0)
		{
			ray->length = length;
			ray->shape = (t_shape *)sphere;
			ray->shape_type = SHAPE_SPHERE;
		}
	}
}

void	min_plane_intersect_dist(t_ray *ray, const t_plane *plane)
{
	double	length;

	length = -vec3_dot(vec3_sub(ray->start, plane->coords), plane->normal) / \
		vec3_dot(ray->dir, plane->normal);
	if (length < ray->length && length >= 0)
	{
		ray->length = length;
		ray->shape = (t_shape *)plane;
		ray->shape_type = SHAPE_PLANE;
	}
}

void	min_disc_intersect_dist(t_ray *ray, const t_disc *disc)
{
	double	length;
	t_vec3	hitp;
	t_vec3	hitp_sub_orig;

	length = -vec3_dot(vec3_sub(ray->start, disc->coords), disc->normal) / \
		vec3_dot(ray->dir, disc->normal);
	hitp = vec3_add(ray->start, vec3_muls(ray->dir, length));
	hitp_sub_orig = vec3_sub(hitp, disc->coords);
	if (length < ray->length && length >= 0
		&& vec3_dot(hitp_sub_orig, hitp_sub_orig) \
			<= disc->radius * disc->radius)
	{
		ray->length = length;
		ray->shape = (t_shape *)disc;
		ray->shape_type = SHAPE_DISC;
	}
}

void	get_shape_intersect_dist(t_minirt *m, t_ray *ray, const t_shape *skip)
{
	size_t	i;

	i = (size_t) - 1;
	while (++i < m->spheres_length)
		if ((t_shape *)&m->spheres[i] != skip)
			min_sphere_intersect_dist(ray, &m->spheres[i]);
	i = (size_t) - 1;
	while (++i < m->planes_length)
		if ((t_shape *)&m->planes[i] != skip)
			min_plane_intersect_dist(ray, &m->planes[i]);
	i = (size_t) - 1;
	while (++i < m->cylinders_length)
		if ((t_shape *)&m->cylinders[i] != skip)
			min_cylinder_intersect_dist(ray, &m->cylinders[i]);
	i = (size_t) - 1;
	while (++i < m->discs_length)
		if ((t_shape *)&m->discs[i] != skip)
			min_disc_intersect_dist(ray, &m->discs[i]);
}
