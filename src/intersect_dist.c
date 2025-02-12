/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_dist.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 12:29:56 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/12 18:32:28 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "ray.h"
#include <math.h>

static void	min_sphere_intersect_dist(t_ray *ray, const t_sphere *sphere)
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
		length = fabs(-b - sqrt(discriminant)) / (2. * 1);
		if (length < ray->length)
		{
			ray->length = length;
			ray->shape = (t_shape *)sphere;
			ray->shape_type = SHAPE_SPHERE;
		}
	}
}

static void	min_plane_intersect_dist(t_ray *ray, const t_plane *plane)
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

static void	min_cylinder_intersect_dist(t_ray *ray, const t_cylinder *cylinder)
{
	double	length;
	double	a;
	double	b;
	double	c;
	double	discriminant;
	double	axis_dot_dir;
	t_vec3	cap;
	t_vec3	top;
	t_vec3	bot;

	cap = vec3_muls(cylinder->axis, cylinder->height / 2.);
	top = vec3_add(cylinder->coords, cap);
	bot = vec3_sub(cylinder->coords, cap);

	axis_dot_dir = vec3_dot(cylinder->axis, ray->dir);
	t_vec3 rl = vec3_sub(ray->start, bot);
	double axis_dot_rl = vec3_dot(cylinder->axis, rl);

	a = 1. - axis_dot_dir * axis_dot_dir;
	b = 2. * (vec3_dot(ray->dir, rl) - vec3_dot(cylinder->axis, ray->dir) * axis_dot_rl);
	c = vec3_dot(rl, rl) - axis_dot_rl * axis_dot_rl - cylinder->radius * cylinder->radius;

	discriminant = b * b - 4 * a * c;

	if (discriminant >= 0)
	{
		double b_term = b <= 0. ? -b + sqrt(discriminant) : -b - sqrt(discriminant);
		#if 1
		double hit1 = b_term / (2. * a);
		double hit2 = (2. * c) / b_term;
		#else
		double hit1 = fabs(b_term / (2. * a));
		double hit2 = fabs((2. * c) / b_term);
		#endif

		t_vec3 hitp1 = vec3_add(ray->start, vec3_muls(ray->dir, hit1));
		t_vec3 hitp2 = vec3_add(ray->start, vec3_muls(ray->dir, hit2));
		double hit_offset1 = vec3_dot(vec3_sub(top, hitp1), cylinder->axis);
		double hit_offset2 = vec3_dot(vec3_sub(top, hitp2), cylinder->axis);

		bool valid1 = hit1 >= 0 && hit_offset1 >= 0 && hit_offset1 < cylinder->height;
		bool valid2 = hit2 >= 0 && hit_offset2 >= 0 && hit_offset2 < cylinder->height;

		if (!valid1 && !valid2)
			return ;
		else if (valid1 && valid2)
			length = fmin(hit1, hit2);
		else if (valid1)
			length = hit1;
		else
			length = hit2;

		if (length < ray->length)
		{
			ray->length = length;
			ray->shape = (t_shape *)cylinder;
			ray->shape_type = SHAPE_CYLINDER;
		}
	}
}

static void	min_disc_intersect_dist(t_ray *ray, const t_disc *disc)
{
	double	length;
	t_vec3	hitp;
	t_vec3	hitp_sub_orig;

	length = -vec3_dot(vec3_sub(ray->start, disc->coords), disc->normal) / \
		vec3_dot(ray->dir, disc->normal);
	hitp = vec3_add(ray->start, vec3_muls(ray->dir, length));
	hitp_sub_orig = vec3_sub(hitp, disc->coords);
	if (length < ray->length && length >= 0
		&& vec3_dot(hitp_sub_orig, hitp_sub_orig) <= disc->radius * disc->radius)
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
