/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_attributes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 19:03:30 by lfiestas          #+#    #+#             */
/*   Updated: 2025/01/31 17:57:57 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>

bool	parse_ambient_light(t_minirt *m, const char *line)
{
	while (ft_isspace(*line))
		++line;
	line = parse_float(m, &m->ambient_light_ratio, line, ' ');
	assert_range(m, (t_vec3){m->ambient_light_ratio, 0., 1.}, \
		"Ambient lighting ratio");
	line = parse_float(m, &m->ambient_light_color.r, line, ',');
	assert_range(m, (t_vec3){m->ambient_light_color.r, 0, 255}, \
		"Ambient light red component");
	line = parse_float(m, &m->ambient_light_color.g, line, ',');
	assert_range(m, (t_vec3){m->ambient_light_color.g, 0, 255}, \
		"Ambient light green component");
	line = parse_float(m, &m->ambient_light_color.b, line, '\0');
	return (assert_range(m, (t_vec3){m->ambient_light_color.b, 0, 255}, \
		"Ambient light blue component"));
}

bool	parse_camera(t_minirt *m, const char *line)
{
	while (ft_isspace(*line))
		++line;
	line = parse_float(m, &m->camera_coords.x, line, ',');
	line = parse_float(m, &m->camera_coords.y, line, ',');
	line = parse_float(m, &m->camera_coords.z, line, ' ');
	line = parse_float(m, &m->camera_orientation.x, line, ',');
	assert_range(m, (t_vec3){m->camera_orientation.x, -1, 1}, \
		"Camera orientation x component");
	line = parse_float(m, &m->camera_orientation.y, line, ',');
	assert_range(m, (t_vec3){m->camera_orientation.y, -1, 1}, \
		"Camera orientation y component");
	line = parse_float(m, &m->camera_orientation.z, line, ' ');
	assert_range(m, (t_vec3){m->camera_orientation.z, -1, 1}, \
		"Camera orientation z component");
	if (fabs(vec3_length(m->camera_orientation) - 1.) <= .00001)
		ft_putendl_fd("Warning\nUnnormalized camera orientation", 2);
	m->camera_orientation = vec3_normalize(m->camera_orientation);
	line = parse_float(m, &m->camera_field_of_view, line, '\0');
	return (assert_range(m, (t_vec3){m->camera_field_of_view, 0, 180},
		"Camera horizontal vield of view"));
}

bool	parse_light(t_minirt *m, const char *line)
{
	while (ft_isspace(*line))
		++line;
	line = parse_float(m, &m->light_coords.x, line, ',');
	line = parse_float(m, &m->light_coords.y, line, ',');
	line = parse_float(m, &m->light_coords.z, line, ' ');
	line = parse_float(m, &m->light_ratio, line, '\0');
	return (assert_range(m, (t_vec3){m->light_ratio, 0., 1.}, \
		"Light brightness ratio"));
}
