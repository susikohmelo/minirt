/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_shapes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 20:00:45 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/06 19:45:58 by ljylhank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>

void	parse_sphere(t_minirt *m, const char *line)
{
	t_sphere	sphere;

	while (ft_isspace(*line))
		++line;
	line = parse_float(m, &sphere.coords.x, line, ',');
	line = parse_float(m, &sphere.coords.y, line, ',');
	line = parse_float(m, &sphere.coords.z, line, ' ');
	line = parse_float(m, &sphere.radius, line, ' ');
	sphere.radius /= 2.;
	line = parse_float(m, &sphere.color.r, line, ',');
	assert_range(m, vec3(sphere.color.r, 0, 255), "Sphere red component");
	line = parse_float(m, &sphere.color.g, line, ',');
	assert_range(m, vec3(sphere.color.g, 0, 255), "Sphere green component");
	line = parse_float(m, &sphere.color.b, line, -1);
	assert_range(m, vec3(sphere.color.b, 0, 255), "Sphere blue component");
	sphere.color = vec3_divs(sphere.color, 255);
	sphere.texture = parse_texture(m, &line);
	m->spheres[m->spheres_length++] = sphere;
}

void	parse_plane(t_minirt *m, const char *line)
{
	t_plane	plane;

	while (ft_isspace(*line))
		++line;
	line = parse_float(m, &plane.coords.x, line, ',');
	line = parse_float(m, &plane.coords.y, line, ',');
	line = parse_float(m, &plane.coords.z, line, ' ');
	line = parse_float(m, &plane.normal.x, line, ',');
	assert_range(m, vec3(plane.normal.x, -1, 1), "Plane normal x component");
	line = parse_float(m, &plane.normal.y, line, ',');
	assert_range(m, vec3(plane.normal.y, -1, 1), "Plane normal y component");
	line = parse_float(m, &plane.normal.z, line, ' ');
	assert_range(m, vec3(plane.normal.z, -1, 1), "Plane normal z component");
	if (fabs(vec3_length(plane.normal) - 1.) >= .001)
		ft_putendl_fd("Warning\nUnnormalized camera orientation", 2);
	plane.normal = vec3_normalize(plane.normal);
	line = parse_float(m, &plane.color.r, line, ',');
	assert_range(m, vec3(plane.color.r, 0, 255), "Plane red component");
	line = parse_float(m, &plane.color.g, line, ',');
	assert_range(m, vec3(plane.color.g, 0, 255), "Plane green component");
	line = parse_float(m, &plane.color.b, line, -1);
	assert_range(m, vec3(plane.color.b, 0, 255), "Plane blue component");
	plane.color = vec3_divs(plane.color, 255);
	plane.texture = parse_texture(m, &line);
	m->planes[m->planes_length++] = plane;
}

void	parse_cylinder(t_minirt *m, const char *line)
{
	t_cylinder	cylinder;

	while (ft_isspace(*line))
		++line;
	line = parse_float(m, &cylinder.coords.x, line, ',');
	line = parse_float(m, &cylinder.coords.y, line, ',');
	line = parse_float(m, &cylinder.coords.z, line, ' ');
	line = parse_float(m, &cylinder.axis.x, line, ',');
	assert_range(m, vec3(cylinder.axis.x, -1, 1), "Cylinder axis x component");
	line = parse_float(m, &cylinder.axis.y, line, ',');
	assert_range(m, vec3(cylinder.axis.y, -1, 1), "Cylinder axis y component");
	line = parse_float(m, &cylinder.axis.z, line, ' ');
	assert_range(m, vec3(cylinder.axis.z, -1, 1), "Cylinder axis z component");
	if (fabs(vec3_length(cylinder.axis) - 1.) >= .001)
		ft_putendl_fd("Warning\nUnnormalized cylinder axis", 2);
	cylinder.axis = vec3_normalize(cylinder.axis);
	line = parse_float(m, &cylinder.diameter, line, ' ');
	line = parse_float(m, &cylinder.height, line, ' ');
	line = parse_float(m, &cylinder.color.r, line, ',');
	assert_range(m, vec3(cylinder.color.r, 0, 255), "Cylinder red component");
	line = parse_float(m, &cylinder.color.g, line, ',');
	assert_range(m, vec3(cylinder.color.g, 0, 255), "Cylinder green component");
	line = parse_float(m, &cylinder.color.b, line, -1);
	assert_range(m, vec3(cylinder.color.b, 0, 255), "Cylinder blue component");
	cylinder.color = vec3_divs(cylinder.color, 255);
	cylinder.texture = parse_texture(m, &line);
	m->cylinders[m->cylinders_length++] = cylinder;
}
