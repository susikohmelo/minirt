/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_dist.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 12:29:56 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/11 14:56:38 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "ray.h"
#include <math.h>

#define EPSILON 1e-8

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
		length = (-b - sqrt(discriminant)) / (2. * 1); // TODO square this and calculate sqrt() lazily
		if (length < ray->length && length >= 0) // TODO square these too
		{
			ray->length = length; // and the sqrt() would go here
			ray->shape = (t_shape *)sphere;
			ray->shape_type = SHAPE_SPHERE;
		}
	}
}

// TODO hit position is probably wrong here too!
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

// static void	min_disc_intersect_dist(
// 	t_ray *ray, const t_cylinder *cylinder, t_vec3 top, t_vec3 bot)
// {
// 	t_ray	disc_ray;
// 	t_plane	disc_top;
// 	t_plane	disc_bot;
// 	double	length;
//
// 	length = INFINITY;
// 	disc_ray = (t_ray){ray->start, ray->dir, INFINITY, NULL, SHAPE_NO_SHAPE};
//
// 	disc_top = (t_plane){.coords = top, .normal = cylinder->axis};
// 	min_plane_intersect_dist(&disc_ray, &disc_top);
// 	if (vec3_length(vec3_sub(vec3_muls(disc_ray.dir, disc_ray.length), top)) <= cylinder->radius) // TODO square these
// 		length = disc_ray.length;
//
// 	disc_bot = (t_plane){.coords = bot, .normal = cylinder->axis};
// 	min_plane_intersect_dist(&disc_ray, &disc_bot);
// 	if (vec3_length(vec3_sub(vec3_muls(disc_ray.dir, disc_ray.length), bot)) <= cylinder->radius) // TODO square these
// 		length = fmin(length, disc_ray.length);
//
// 	if (length < ray->length && length >= 0)
// 	{
// 		ray->length = length;
// 		ray->shape = (t_shape *)cylinder;
// 		ray->shape_type = SHAPE_PLANE;
// 	}
// }

#include <assert.h>

// static void	min_cap_intersect_dist(
// 	t_ray *ray, const t_cylinder *cylinder, t_vec3 cap)
// {
// 	double	length;
// 	t_plane	plane;
// 	t_ray	disc_ray;
//
// 	length = INFINITY;
// 	disc_ray = (t_ray){ray->start, ray->dir, INFINITY, NULL, SHAPE_NO_SHAPE};
// 	plane = (t_plane){.coords = cap, .normal = cylinder->axis};
// 	min_plane_intersect_dist(&disc_ray, &plane);
// 	t_vec3 offset = vec3_sub(vec3_muls(disc_ray.dir, disc_ray.length), cap);
// 	if (vec3_dot(offset, offset) <= cylinder->radius * cylinder->radius)
// 		length = disc_ray.length;
//
// 	if (length < ray->length && length >= 0)
// 	{
// 		ray->length = length;
// 		ray->shape = (t_shape *)cylinder;
// 		ray->shape_type = SHAPE_PLANE;
// 	}
// }

#if 0
void	min_cylinder_intersect_dist(t_ray *ray, const t_cylinder *cylinder)
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
		bool valid1 = true;
		bool valid2 = true;
		double b_term = b <= 0. ? -b + sqrt(discriminant) : -b - sqrt(discriminant);
		double hit_far = b_term / (2. * a);
		double hit_near = (2. * c) / b_term;
		// double hit_far = (-b + sqrt(discriminant)) / (2. * a);
		// double hit_near = (-b - sqrt(discriminant)) / (2. * a);

		t_vec3 hitp1 = vec3_add(ray->start, vec3_muls(ray->dir, hit_near));
		t_vec3 hitp2 = vec3_add(ray->start, vec3_muls(ray->dir, hit_far));
		double hoffset1 = vec3_dot(vec3_sub(top, hitp1), cylinder->axis);
		double hoffset2 = vec3_dot(vec3_sub(top, hitp2), cylinder->axis);

		if (hit_near < 0. || hoffset1 <= 0. || hoffset1 > cylinder->height)
			valid1 = false;
		if (hit_far < 0. || hoffset2 <= 0. || hoffset2 > cylinder->height)
			valid2 = false;

		if (!valid1 && !valid2) // ray never hits cylinder's curved surface, (watching trough)
		{
			if (axis_dot_rl <= 0.)
				min_cap_intersect_dist(ray, cylinder, top);
			else if (axis_dot_rl >= cylinder->height)
				min_cap_intersect_dist(ray, cylinder, bot);
			else // inside cylinder
			{
				t_ray	disc_near = (t_ray){ray->start, ray->dir, INFINITY, NULL, SHAPE_NO_SHAPE};
				t_ray	disc_far  = (t_ray){ray->start, ray->dir, INFINITY, NULL, SHAPE_NO_SHAPE};
				min_cap_intersect_dist(&disc_near, cylinder, top);
				min_cap_intersect_dist(&disc_far, cylinder, bot);
				valid1 = !isinf(disc_near.length);
				valid2 = !isinf(disc_near.length);
				if (valid1)
				{
					if (valid2)
						if (hit_far < hit_near)
							hit_near = hit_far;
					if (hit_near < ray->length && hit_near >= 0)
					{
						ray->length = hit_near;
						ray->shape = (t_shape *)cylinder;
						ray->shape_type = SHAPE_PLANE;
					}
				}
				else if (valid2 && hit_far < ray->length && hit_far >= 0)
				{
					ray->length = hit_far;
					ray->shape = (t_shape *)cylinder;
					ray->shape_type = SHAPE_PLANE;
				}
			}
		}
		else if (valid1 ^ valid2) // intersecting with disc and surface
		{
			if (valid2)
			{
				hitp1 = hitp2; // TODO do we need this??
				hoffset1 = hoffset2; // TODO do we need this??
				hit_near = hit_far;
			}
			t_ray	disc_top = (t_ray){ray->start, ray->dir, INFINITY, NULL, SHAPE_NO_SHAPE};
			t_ray	disc_bot  = (t_ray){ray->start, ray->dir, INFINITY, NULL, SHAPE_NO_SHAPE};
			min_cap_intersect_dist(&disc_top, cylinder, top);
			min_cap_intersect_dist(&disc_bot, cylinder, bot);
			double cap_length = fmin(fmax(0, disc_top.length), fmax(0, disc_bot.length));
			length = fmin(cap_length, hit_near);
			if (length < ray->length && length >= 0)
			{
				ray->length = length;
				ray->shape = (t_shape *)cylinder;
				if (cap_length < hit_near)
					ray->shape_type = SHAPE_PLANE;
				else
					ray->shape_type = SHAPE_CYLINDER;
			}
		}
		else
		{
			length = fmin(hit_near, hit_far);
			if (length < ray->length && length >= 0)
			{
				ray->length = length;
				ray->shape = (t_shape *)cylinder;
				ray->shape_type = SHAPE_CYLINDER;
			}
		}
	}
	else // watching directly trough cylinder, probably not necessary assuming 'ghost' cylinder being infinite
	{
		t_ray	disc_near = (t_ray){ray->start, ray->dir, INFINITY, NULL, SHAPE_NO_SHAPE};
		t_ray	disc_far  = (t_ray){ray->start, ray->dir, INFINITY, NULL, SHAPE_NO_SHAPE};
		min_cap_intersect_dist(&disc_near, cylinder, top);
		min_cap_intersect_dist(&disc_far, cylinder, bot);
		length = fmin(fmax(0, disc_near.length), fmax(0, disc_far.length));

		if (length < ray->length && length >= 0)
		{
			ray->length = length;
			ray->shape = (t_shape *)cylinder;
			ray->shape_type = SHAPE_PLANE;
		}
	}
}
#endif

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
		// double b_term = b <= 0. ? -b + sqrt(discriminant) : -b - sqrt(discriminant);
		// double hit1 = b_term / (2. * a);
		// double hit2 = (2. * c) / b_term;

		double hit1 = (-b + sqrt(discriminant)) / (2. * a);
		double hit2 = (-b - sqrt(discriminant)) / (2. * a);

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
	#if 0
	length = -vec3_dot(vec3_sub(ray->start, plane->coords), plane->normal) / \
		vec3_dot(ray->dir, plane->normal);
	if (length < ray->length && length >= 0)
	#endif
	double	length;
	t_vec3	hitp;
	t_vec3	hitp_sub_orig;

	length = -vec3_dot(vec3_sub(ray->start, disc->coords), disc->normal) / \
		vec3_dot(ray->dir, disc->normal);
	hitp = vec3_add(ray->start, vec3_muls(ray->dir, length));
	hitp_sub_orig = vec3_sub(hitp, disc->coords);
	if (length < ray->length && length >= 0
		&& vec3_dot(hitp_sub_orig, hitp_sub_orig) < disc->radius * disc->radius)
	{
		ray->length = length;
		ray->shape = (t_shape *)disc;
		ray->shape_type = SHAPE_DISC;
	}
}

void	get_shape_intersect_dist(t_minirt *m, t_ray *ray, const t_shape *skip)
{
	size_t	i;

	mrt_debug(m);

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
