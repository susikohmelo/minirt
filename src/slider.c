/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   slider.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 17:38:50 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/18 17:41:12 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>

static void	render_slider(t_minirt *m, double value)
{
	size_t	x;
	size_t	y;

	value = fmin(fmax(value, 0), 1);
	y = m->gui_line * CHAR_HEIGHT - 1;
	while (++y < (m->gui_line + 1) * CHAR_HEIGHT)
	{
		x = -1;
		while (++x < value * LINE_LENGTH * CHAR_WIDTH)
		{
			m->gui_text->pixels[4 * (y * m->gui_text->width + x) + 0] = 0x60;
			m->gui_text->pixels[4 * (y * m->gui_text->width + x) + 1] = 0x60;
			m->gui_text->pixels[4 * (y * m->gui_text->width + x) + 2] = 0x60;
			m->gui_text->pixels[4 * (y * m->gui_text->width + x) + 3] = 0xFF;
		}
	}
}

static void	render_slider2(t_minirt *m, double value)
{
	size_t	x;
	size_t	y;

	value = fmin(fmax(value, -1), 1);
	y = m->gui_line * CHAR_HEIGHT - 1;
	while (++y < (m->gui_line + 1) * CHAR_HEIGHT)
	{
		x = (1 + (value < 0) * value) * LINE_LENGTH * CHAR_WIDTH / 2 - 1;
		while (++x < (.5 * value + .5) * LINE_LENGTH * CHAR_WIDTH)
		{
			m->gui_text->pixels[4 * (y * m->gui_text->width + x) + 0] = 0x60;
			m->gui_text->pixels[4 * (y * m->gui_text->width + x) + 1] = 0x60;
			m->gui_text->pixels[4 * (y * m->gui_text->width + x) + 2] = 0x60;
			m->gui_text->pixels[4 * (y * m->gui_text->width + x) + 3] = 0xFF;
		}
		while (++x < LINE_LENGTH * CHAR_WIDTH / 2)
		{
			m->gui_text->pixels[4 * (y * m->gui_text->width + x) + 0] = 0x60;
			m->gui_text->pixels[4 * (y * m->gui_text->width + x) + 1] = 0x60;
			m->gui_text->pixels[4 * (y * m->gui_text->width + x) + 2] = 0x60;
			m->gui_text->pixels[4 * (y * m->gui_text->width + x) + 3] = 0xFF;
		}
	}
}

void	render_value(
	t_minirt *m, const char *value_name, double value, double scale)
{
	char	line[LINE_LENGTH + 1];
	char	fbuf[32];

	render_slider(m, value);
	f_to_str(fbuf, scale * value)[FLOAT_WIDTH] = '\0';
	ft_memset(line, ' ', sizeof line);
	ft_memcpy(line, value_name, ft_strlen(value_name));
	ft_memcpy(line + LINE_LENGTH - ft_strlen(fbuf), fbuf, FLOAT_WIDTH);
	line[LINE_LENGTH] = '\0';
	render_string(m, line);
}

static void	render_normalized_value(
	t_minirt *m, const char *value_name, double value)
{
	char	line[LINE_LENGTH + 1];
	char	fbuf[32];

	render_slider2(m, value);
	f_to_str(fbuf, value)[FLOAT_WIDTH] = '\0';
	ft_memset(line, ' ', sizeof line);
	ft_memcpy(line, value_name, ft_strlen(value_name));
	ft_memcpy(line + LINE_LENGTH - ft_strlen(fbuf), fbuf, FLOAT_WIDTH);
	line[LINE_LENGTH] = '\0';
	render_string(m, line);
}

void	render_normalized_vector_value(
	t_minirt *m, const char *value_name, t_vec3 value)
{
	char	name[LINE_LENGTH + 1];
	size_t	name_length;

	name_length = ft_strlen(value_name);
	ft_memcpy(name, value_name, name_length);
	name[name_length + 0] = ' ';
	name[name_length + 2] = '\0';
	name[name_length + 1] = 'X';
	render_normalized_value(m, name, value.x);
	name[name_length + 1] = 'Y';
	render_normalized_value(m, name, value.y);
	name[name_length + 1] = 'Z';
	render_normalized_value(m, name, value.z);
}
