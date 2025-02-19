/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_dist.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 12:29:56 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/19 14:29:02 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "ray.h"
#include <math.h>

void	min_light_intersect_dist(t_ray *ray, const t_light *light)
{
	t_vec3	lstart;
	double	b;
	double	c;
	double	discriminant;
	double	length;

	lstart = vec3_sub(ray->start, light->coords);
	b = 2 * vec3_dot(ray->dir, lstart);
	c = vec3_dot(lstart, lstart) - (light->brightness + .5);
	discriminant = b * b - 4 * 1 * c;
	if (discriminant >= 0)
	{
		length = (-b - sqrt(discriminant)) / (2. * 1);
		if (length < ray->length && length >= 0)
		{
			ray->length = length;
			ray->shape = (t_shape *)light;
			ray->shape_type = SHAPE_LIGHT;
		}
	}
}

void	get_light_intersect_dist(t_minirt *m, t_ray *ray)
{
	size_t	i;

	i = (size_t) - 1;
	while (++i < m->lights_length)
		min_light_intersect_dist(ray, &m->lights[i]);
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
