/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_intersect_dist.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 18:21:00 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/18 18:26:28 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>

static void	cylinder_hit(
	t_ray *ray, const t_cylinder *cylinder, t_cylinder_intersect_data c)
{
	c.b_term = -c.b - sqrt(c.discriminant);
	if (c.b <= 0.)
		c.b_term = -c.b + sqrt(c.discriminant);
	c.h1 = c.b_term / (2. * c.a);
	c.h2 = (2. * c.c) / c.b_term;
	c.hitp1 = vec3_add(ray->start, vec3_muls(ray->dir, c.h1));
	c.hitp2 = vec3_add(ray->start, vec3_muls(ray->dir, c.h2));
	c.hoffset1 = vec3_dot(vec3_sub(c.top, c.hitp1), cylinder->axis);
	c.hoffset2 = vec3_dot(vec3_sub(c.top, c.hitp2), cylinder->axis);
	c.valid1 = (c.h1 >= 0 && c.hoffset1 >= 0 && c.hoffset1 < cylinder->height);
	c.valid2 = (c.h2 >= 0 && c.hoffset2 >= 0 && c.hoffset2 < cylinder->height);
	if (!c.valid1 && !c.valid2)
		return ;
	else if (c.valid1 && c.valid2)
		c.length = fmin(c.h1, c.h2);
	else if (c.valid1)
		c.length = c.h1;
	else
		c.length = c.h2;
	if (c.length < ray->length)
	{
		ray->length = c.length;
		ray->shape = (t_shape *)cylinder;
		ray->shape_type = SHAPE_CYLINDER;
	}
}

void	min_cylinder_intersect_dist(t_ray *ray, const t_cylinder *cylinder)
{
	t_cylinder_intersect_data	c;

	c.cap = vec3_muls(cylinder->axis, cylinder->height / 2.);
	c.top = vec3_add(cylinder->coords, c.cap);
	c.bot = vec3_sub(cylinder->coords, c.cap);
	c.axis_dot_dir = vec3_dot(cylinder->axis, ray->dir);
	c.rl = vec3_sub(ray->start, c.bot);
	c.axis_dot_rl = vec3_dot(cylinder->axis, c.rl);
	c.a = 1. - c.axis_dot_dir * c.axis_dot_dir;
	c.b = 2. * (vec3_dot(ray->dir, c.rl) - \
		vec3_dot(cylinder->axis, ray->dir) * c.axis_dot_rl);
	c.c = vec3_dot(c.rl, c.rl) - \
		c.axis_dot_rl * c.axis_dot_rl - cylinder->radius * cylinder->radius;
	c.discriminant = c.b * c.b - 4 * c.a * c.c;
	if (c.discriminant >= 0)
		cylinder_hit(ray, cylinder, c);
}
