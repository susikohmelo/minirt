/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_dist.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 12:29:56 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/06 12:40:55 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>



#include <stdio.h>

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
	if (discriminant >= 0.)
	{
		length = (-b - sqrt(discriminant)) / (2. * 1);
		//ray->length = fmin(ray->length, length);
		if (length <= ray->length && length >= 0.)
		{
			ray->length = length;
			ray->shape = sphere;
			ray->shape_type = SHAPE_SPHERE;
		}
	}
	//else
	//	length = INFINITY;
	//ray->length = length;
	// ray->length = fmin(ray->length, length);
	// printf("%g\n", ray->length);
}

// void	min_plane_intersect_dist(t_ray ray, t_plane plane)
// {
// 	(void)ray;
// 	(void)plane;
// 	return (INFINITY);
// }
//
// void	min_cylinder_intersect_dist(t_ray ray, t_cylinder cylinder)
// {
// 	(void)ray;
// 	(void)cylinder;
// 	return (INFINITY);
// }
//
