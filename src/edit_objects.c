/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edit_objects.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 10:44:57 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/19 12:54:35 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>

void	edit_common_shape_attributes(t_minirt *m, t_shape *s, double x)
{
	if (m->moving_slider == 1)
		s->color.r = x;
	if (m->moving_slider == 2)
		s->color.g = x;
	if (m->moving_slider == 3)
		s->color.b = x;
	if (m->moving_slider == 4)
		s->roughness = x;
}

static void	edit_light(t_minirt *m, t_light *light, double x)
{
	if (m->moving_slider == 1)
		light->brightness = x;
	if (m->moving_slider == 2)
		light->color_value.r = x;
	if (m->moving_slider == 3)
		light->color_value.g = x;
	if (m->moving_slider == 4)
		light->color_value.b = x;
	light->color = vec3_muls(light->color_value, light->brightness);
}

static void	edit_global_attributes(t_minirt *m, double x)
{
	if (m->moving_slider == 1)
		m->ambient_light_ratio = x;
	if (m->moving_slider == 2)
		m->ambient_light_color.r = x;
	if (m->moving_slider == 3)
		m->ambient_light_color.g = x;
	if (m->moving_slider == 4)
		m->ambient_light_color.b = x;
	if (m->moving_slider == 5)
		m->camera_field_of_view = 180 * x;
	if (1 <= m->moving_slider && m->moving_slider <= 4)
		m->ambient_light = vec3_muls(\
			m->ambient_light_color, m->ambient_light_ratio);
}

void	edit_objects(t_minirt *m, double x)
{
	x = fmin(fmax(x / (LINE_LENGTH * CHAR_WIDTH), 0), 1);
	if (m->shape_type == SHAPE_SPHERE)
		edit_sphere(m, m->shape, x);
	if (m->shape_type == SHAPE_PLANE)
		edit_plane(m, m->shape, x);
	if (m->shape_type == SHAPE_CYLINDER)
		edit_cylinder(m, m->shape, x);
	if (m->shape_type == SHAPE_LIGHT)
		edit_light(m, (t_light *)m->shape, x);
	if (m->shape_type == SHAPE_GLOBAL_ATTRIBUTES)
		edit_global_attributes(m, x);
	ft_memset(m->valid_pixel, false, m->valid_pixel_len);
}
