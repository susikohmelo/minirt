/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_attributes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 19:03:30 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/08 12:52:02 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

bool	parse_ambient_light(t_minirt *m, const char *line)
{
	double	ratio;

	line = trim_left(line);
	line = parse_float(m, &ratio, line, ' ');
	assert_range(m, vec3(ratio, 0., 1.), "Ambient lighting ratio");
	line = parse_float(m, &m->ambient_light.r, line, ',');
	assert_range(m, vec3(m->ambient_light.r, 0, 255), \
		"Ambient light red component");
	line = parse_float(m, &m->ambient_light.g, line, ',');
	assert_range(m, vec3(m->ambient_light.g, 0, 255), \
		"Ambient light green component");
	line = parse_float(m, &m->ambient_light.b, line, '\0');
	assert_range(m, vec3(m->ambient_light.b, 0, 255), \
		"Ambient light blue component");
	m->ambient_light = vec3_muls(m->ambient_light, ratio / 255);
	return (true);
}

bool	parse_camera(t_minirt *m, const char *line)
{
	line = trim_left(line);
	line = parse_float(m, &m->camera_coords.x, line, ',');
	line = parse_float(m, &m->camera_coords.y, line, ',');
	line = parse_float(m, &m->camera_coords.z, line, ' ');
	line = parse_float(m, &m->camera_orientation.x, line, ',');
	assert_range(m, vec3(m->camera_orientation.x, -1, 1), \
		"Camera orientation x component");
	line = parse_float(m, &m->camera_orientation.y, line, ',');
	assert_range(m, vec3(m->camera_orientation.y, -1, 1), \
		"Camera orientation y component");
	line = parse_float(m, &m->camera_orientation.z, line, ' ');
	assert_range(m, vec3(m->camera_orientation.z, -1, 1), \
		"Camera orientation z component");
	m->camera_orientation = expect_normalized(m->camera_orientation, \
		"camera orientation");
	line = parse_float(m, &m->camera_field_of_view, line, '\0');
	assert_range(m, vec3(m->camera_field_of_view, 0, 180),
		"Camera horizontal vield of view");
	return (true);
}

bool	parse_lights(t_minirt *m, const char *line)
{
	t_light	light;
	double	ratio;

	line = trim_left(line);
	line = parse_float(m, &light.coords.x, line, ',');
	line = parse_float(m, &light.coords.y, line, ',');
	line = parse_float(m, &light.coords.z, line, ' ');
	line = parse_float(m, &ratio, line, ' ');
	assert_range(m, vec3(ratio, 0., 1.), "Light brightness ratio");
	line = parse_float(m, &light.color.r, line, ',');
	assert_range(m, vec3(light.color.r, 0, 255), "Light red component");
	line = parse_float(m, &light.color.g, line, ',');
	assert_range(m, vec3(light.color.g, 0, 255), "Light green component");
	line = parse_float(m, &light.color.b, line, '\0');
	assert_range(m, vec3(light.color.b, 0, 255), "Light blue component");
	light.color = vec3_muls(light.color, ratio / 255);
	m->lights[m->lights_length++] = light;
	return (true);
}
