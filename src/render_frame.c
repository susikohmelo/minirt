/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_frame.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 21:06:44 by ljylhank          #+#    #+#             */
/*   Updated: 2025/02/14 18:00:23 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

size_t	fast_strlcat(
	char buf[static LINE_LENGTH + 1], size_t length, const char *s)
{
	return (length + ft_strlcpy(buf + length, s, LINE_LENGTH + 1 - length));
}

size_t	put_float(char buf[static LINE_LENGTH + 1], size_t length, double f)
{
	static char		field_pad[] =  "                                          ";
	char			fbuf[32];
	size_t			out_length;

	field_pad[FLOAT_WIDTH] = '\0';
	f_to_str(fbuf, f)[FLOAT_WIDTH] = '\0';
	out_length = fast_strlcat(buf, length, field_pad);
	ft_memcpy(buf + length, fbuf, FLOAT_WIDTH);
	return (out_length);
}

size_t	put_field(char buf[static LINE_LENGTH + 1], size_t length, const char *str)
{
	static char		field_pad[] =  "                                          ";
	const size_t	field_width = 16;
	size_t			out_length;

	field_pad[field_width] = '\0';
	out_length = fast_strlcat(buf, length, field_pad);
	ft_memcpy(buf + length, str, ft_strlen(str));
	return (out_length);
}

void	render_value(t_minirt *m, const char *value_name, double value)
{
	char	line[LINE_LENGTH + 1];
	char	fbuf[32];

	f_to_str(fbuf, value)[FLOAT_WIDTH] = '\0';
	ft_memset(line, ' ', sizeof line);
	ft_memcpy(line, value_name, ft_strlen(value_name));
	ft_memcpy(line + LINE_LENGTH - FLOAT_WIDTH, fbuf, FLOAT_WIDTH);
	line[LINE_LENGTH] = '\0';
}

void	render_common_shape_text(t_minirt *m)
{
	render_value(m, "Red", m->shape->color.r);
	render_value(m, "Green", m->shape->color.g);
	render_value(m, "Blue", m->shape->color.b);
}

void	render_header(t_minirt *m, const char *header)
{
	char	line[LINE_LENGTH + 1];

	ft_memset(line, ' ', sizeof line);
	ft_memcpy(line, header, ft_strlen(header));
	line[LINE_LENGTH - 1] = 'X';
	line[LINE_LENGTH - 0] = '\0';
}

void	render_sphere_text(t_minirt *m)
{
	render_header(m, "Sphere");
	render_common_shape_text(m);
}

void	render_plane_text(t_minirt *m)
{
	render_header(m, "Plane");
	render_common_shape_text(m);
}

void	render_cylinder_text(t_minirt *m)
{
	render_header(m, "Cylinder");
	render_common_shape_text(m);
}

void	render_disc_text(t_minirt *m)
{
	render_header(m, "Cylinder");
	render_common_shape_text(m);
}

void	render_default_text(t_minirt *m)
{
}

void	render_attributes_text(t_minirt *m)
{
	render_header(m, "Environment");

}

void	render_text(t_minirt *m)
{
	if (m->shape_type == SHAPE_SPHERE)
		render_sphere_text(m);
	else if (m->shape_type == SHAPE_PLANE)
		render_plane_text(m);
	else if (m->shape_type == SHAPE_CYLINDER)
		render_cylinder_text(m);
	else if (m->shape_type == SHAPE_DISC)
		render_disc_text(m);
	else if (m->shape_type == SHAPE_GLOBAL_ATTRIBUTES)
		render_attributes_text(m);
	else
		render_default_text(m);
}

void	render_frame(void *minirt)
{
	const char	ones[] = "\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1";
	t_minirt	*m;

	m = minirt;

	cast_rays(minirt);
	m->valid_pixel[m->valid_pixel_i] = true;
	m->valid_pixel_i = (m->valid_pixel_i + 5) & (sizeof m->valid_pixel - 1);
	if (ft_memcmp(m->valid_pixel, ones, sizeof m->valid_pixel) == 0)
	{
		m->double_clicked = false;
		m->resizing = false;
	}
	render_text(m);
}
