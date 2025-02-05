/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_dist.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 12:29:56 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/05 14:48:06 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>

double	sphere_intersect_dist(t_ray ray, t_sphere sphere)
{
	t_vec3	lstart;
	double	a;
	double	b;
	double	c;
	double	discriminant;

	lstart = vec3_sub(ray.start, sphere.coords);
	a = 1;
	b = 2 * vec3_dot(ray.dir, lstart);
	c = vec3_dot(lstart, lstart) - sphere.radius * sphere.radius;
	discriminant = b * b - 4 * a * c;
	if (discriminant >= 0.)
		return ((-b - sqrt(discriminant)) / (2. * a));
	else
		return (INFINITY);
}

double	plane_intersect_dist(t_ray ray, t_plane plane)
{
	(void)ray;
	(void)plane;
	return (INFINITY);
}

double	cylinder_intersect_dist(t_ray ray, t_cylinder cylinder)
{
	(void)ray;
	(void)cylinder;
	return (INFINITY);
}

