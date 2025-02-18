/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edit_shape_objects.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 17:19:12 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/18 20:18:25 by ljylhank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>

void	edit_sphere(t_minirt *m, t_shape *s, double x)
{
	edit_common_shape_attributes(m, s, x);
	if (m->moving_slider == 5)
		((t_sphere *)s)->radius = SCALE * x;
}

void	edit_plane(t_minirt *m, t_shape *plane, double val)
{
	double	t;
	t_vec3	*n;

	edit_common_shape_attributes(m, plane, val);
	if (!(5 <= m->moving_slider && m->moving_slider <= 7))
		return ;
	val = 2 * val - 1;
	n = &((t_plane *)plane)->normal;
	if (fabs(val) == 1 || fabs(n->x) == 1 || fabs(n->y) == 1 || fabs(n->z) == 1)
	{
		t = sqrt((1 - val * val) / 2);
		n->x = (m->moving_slider == 5) * val + (m->moving_slider != 5) * t;
		n->y = (m->moving_slider == 6) * val + (m->moving_slider != 6) * t;
		n->z = (m->moving_slider == 7) * val + (m->moving_slider != 7) * t;
		return ;
	}
	if (m->moving_slider == 5)
		t = sqrt((1 - val * val) / (n->y * n->y + n->z * n->z));
	else if (m->moving_slider == 6)
		t = sqrt((1 - val * val) / (n->x * n->x + n->z * n->z));
	else
		t = sqrt((1 - val * val) / (n->x * n->x + n->y * n->y));
	n->x = (m->moving_slider == 5) * val + (m->moving_slider != 5) * t * n->x;
	n->y = (m->moving_slider == 6) * val + (m->moving_slider != 6) * t * n->y;
	n->z = (m->moving_slider == 7) * val + (m->moving_slider != 7) * t * n->z;
}

void	edit_cylinder(t_minirt *m, t_shape *cylinder, double x)
{
	t_cylinder	*c;
	size_t		i;

	edit_plane(m, cylinder, x);
	c = (t_cylinder *)cylinder;
	if (m->moving_slider == 8)
		c->radius = SCALE * x;
	if (m->moving_slider == 9)
		c->height = SCALE * x;
	i = 2 * (c - m->cylinders);
	m->discs[i].coords = vec3_add(c->coords, vec3_muls(c->axis, c->height / 2));
	m->discs[i].color = c->color;
	m->discs[i].default_rough = c->default_rough;
	m->discs[i].normal = c->axis;
	m->discs[i].radius = c->radius;
	++i;
	m->discs[i].coords = vec3_sub(c->coords, vec3_muls(c->axis, c->height / 2));
	m->discs[i].color = c->color;
	m->discs[i].default_rough = c->default_rough;
	m->discs[i].normal = c->axis;
	m->discs[i].radius = c->radius;
}
