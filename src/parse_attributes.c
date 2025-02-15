/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_attributes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 19:03:30 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/12 12:47:36 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

bool	parse_ambient_light(t_minirt *m, const char *line)
{
	line = trim_left(line);
	line = parse_float(m, &m->ambient_light_ratio, line, ' ');
	expect_range( \
		m, vec3(m->ambient_light_ratio, 0., 1.), "Ambient lighting ratio");
	line = parse_float(m, &m->ambient_light_color.r, line, ',');
	expect_range(m, vec3(m->ambient_light_color.r, 0, 255), \
		"Ambient light red component");
	line = parse_float(m, &m->ambient_light_color.g, line, ',');
	expect_range(m, vec3(m->ambient_light_color.g, 0, 255), \
		"Ambient light green component");
	line = parse_float(m, &m->ambient_light_color.b, line, '\0');
	expect_range(m, vec3(m->ambient_light_color.b, 0, 255), \
		"Ambient light blue component");
	m->ambient_light_color = vec3_divs(m->ambient_light_color, 255);
	m->ambient_light = vec3_muls(m->ambient_light_color, m->ambient_light_ratio);
	return (true);
}

bool	parse_camera(t_minirt *m, const char *line)
{
	line = trim_left(line);
	line = parse_float(m, &m->camera_coords.x, line, ',');
	line = parse_float(m, &m->camera_coords.y, line, ',');
	line = parse_float(m, &m->camera_coords.z, line, ' ');
	line = parse_float(m, &m->camera_orientation.x, line, ',');
	expect_range(m, vec3(m->camera_orientation.x, -1, 1), \
		"Camera orientation x component");
	line = parse_float(m, &m->camera_orientation.y, line, ',');
	expect_range(m, vec3(m->camera_orientation.y, -1, 1), \
		"Camera orientation y component");
	line = parse_float(m, &m->camera_orientation.z, line, ' ');
	expect_range(m, vec3(m->camera_orientation.z, -1, 1), \
		"Camera orientation z component");
	m->camera_orientation = expect_normalized(m, m->camera_orientation, \
		"camera orientation");
	line = parse_float(m, &m->camera_field_of_view, line, '\0');
	expect_range(m, vec3(m->camera_field_of_view, 0, 180),
		"Camera horizontal vield of view");
	return (true);
}

bool	parse_light(t_minirt *m, const char *line)
{
	t_light	light;

	line = trim_left(line);
	line = parse_float(m, &light.coords.x, line, ',');
	line = parse_float(m, &light.coords.y, line, ',');
	line = parse_float(m, &light.coords.z, line, ' ');
	line = parse_float(m, &light.brightness, line, ' ');
	expect_range(m, vec3(light.brightness, 0., 1.), "Light brightness light.brightness");
	line = parse_float(m, &light.color_value.r, line, ',');
	expect_range(m, vec3(light.color_value.r, 0, 255), "Light red component");
	line = parse_float(m, &light.color_value.g, line, ',');
	expect_range(m, vec3(light.color_value.g, 0, 255), "Light green component");
	line = parse_float(m, &light.color_value.b, line, '\0');
	expect_range(m, vec3(light.color_value.b, 0, 255), "Light blue component");
	light.color_value = vec3_divs(light.color_value, 255);
	light.color = vec3_muls(light.color_value, light.brightness);
	m->lights[m->lights_length++] = light;
	return (true);
}
