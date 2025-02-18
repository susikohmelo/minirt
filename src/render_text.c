/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_text.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 17:43:22 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/18 20:27:52 by ljylhank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	render_header(t_minirt *m, const char *header)
{
	char	line[LINE_LENGTH + 1];

	ft_memset(line, ' ', sizeof line);
	ft_memcpy(line, header, ft_strlen(header));
	line[LINE_LENGTH - 1] = 'X';
	line[LINE_LENGTH - 0] = '\0';
	render_string(m, line);
	render_string(m, "");
}

static void	render_attributes_text(t_minirt *m)
{
	char	line[LINE_LENGTH + 1];

	render_header(m, "Environment");
	render_value(m, "Ambient light ratio", m->ambient_light_ratio, 1);
	render_value(m, "Ambient light red", m->ambient_light_color.r, 255);
	render_value(m, "Ambient light green", m->ambient_light_color.g, 255);
	render_value(m, "Ambient light blue", m->ambient_light_color.b, 255);
	render_value(m, "Field of view", m->camera_field_of_view / 180, 180);
	ft_memset(line, ' ', sizeof line);
	ft_memcpy(line, "Show lights", ft_strlen("Show lights"));
	if (m->show_lights)
		ft_memcpy(line + LINE_LENGTH - ft_strlen("yes"), "yes", sizeof "yes");
	else
		ft_memcpy(line + LINE_LENGTH - ft_strlen("no"), "no", sizeof "no");
	render_string(m, line);
}

static void	render_light_text(t_minirt *m)
{
	render_header(m, "Light");
	render_value(m, "Brightness", ((t_light *)m->shape)->brightness, 1);
	render_value(m, "Red", ((t_light *)m->shape)->color_value.r, 255);
	render_value(m, "Green", ((t_light *)m->shape)->color_value.g, 255);
	render_value(m, "Blue", ((t_light *)m->shape)->color_value.b, 255);
}

void	render_text(t_minirt *m)
{
	render_string(m, NULL);
	if (m->shape_type == SHAPE_SPHERE)
		render_sphere_text(m);
	else if (m->shape_type == SHAPE_PLANE)
		render_plane_text(m);
	else if (m->shape_type == SHAPE_CYLINDER)
		render_cylinder_text(m);
	else if (m->shape_type == SHAPE_LIGHT)
		render_light_text(m);
	else if (m->shape_type == SHAPE_GLOBAL_ATTRIBUTES)
		render_attributes_text(m);
	else
		render_string(m, "o");
}
