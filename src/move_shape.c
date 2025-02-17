/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_shape.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 11:37:09 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/17 18:17:15 by ljylhank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>

void	move_shape(t_minirt *m, double x, double y)
{
	t_vec3	delta_x;
	t_vec3	delta_y;
	t_vec3	delta;
	double	length;
	size_t	i;

	x = 2 * x / m->img->width - 1;
	y = 2 * y / m->img->height - 1;
	x *= m->aspect_ratio;
	delta_x = vec3_muls(perpendiculary(m->camera_orientation), m->click_x - x);
	delta_y = vec3_muls(vec3(0,1,0), m->click_y - y);
	delta = vec3_add(delta_x, delta_y);
	length = vec3_length(vec3_sub(m->moving_shape_start, m->camera_coords));
	delta = vec3_muls(delta, length / sqrt(2));
	m->moving_shape->coords = vec3_add(m->moving_shape_start, delta);
	if (m->cylinders <= (t_cylinder *)m->moving_shape
		&& (t_cylinder *)m->moving_shape <= m->cylinders + m->cylinders_length)
	{
		i = 2 * ((t_cylinder *)m->moving_shape - m->cylinders);
		m->discs[i + 0].coords = vec3_add(m->discs[i + 0].coords, delta);
		m->discs[i + 1].coords = vec3_add(m->discs[i + 1].coords, delta);
	}
	redraw(m, true);
}
