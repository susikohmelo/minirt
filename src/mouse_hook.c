/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_hook.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:15:06 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/20 12:04:32 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static t_click	click_type(t_minirt *m, double click_time, int x, int y)
{
	static double	last_click_time;
	double			click_time1;
	const bool		clicked_menu = 0 <= x && 0 <= y
		&& x < LINE_LENGTH && y < (int)m->gui_line;

	click_time1 = last_click_time;
	last_click_time = click_time;
	if (clicked_menu && LINE_LENGTH - 3 <= x && y == 7)
		return (CLICK_SHOW_LIGHTS);
	if (m->shape_type != SHAPE_NO_SHAPE
		&& clicked_menu && 2 <= y)
		return (CLICK_SLIDER);
	if (click_time - click_time1 < .2
		&& m->mouse_x < (int)m->img->width && m->mouse_y < (int)m->img->height)
		return (CLICK_DOUBLE);
	if (LINE_LENGTH - 1 <= x && clicked_menu && m->shape_type != SHAPE_NO_SHAPE)
		return (CLICK_CLOSE_MENU);
	if (clicked_menu && x < 1 && y < 1 && m->shape_type == SHAPE_NO_SHAPE)
		return (CLICK_OPEN_MENU);
	return (CLICK_WORLD);
}

static void	handle_double_click(t_minirt *m)
{
	t_ray	ray;

	ray = cast_ray(m, m->mouse_x, m->mouse_y);
	if (ray.shape_type == SHAPE_DISC)
	{
		m->shape = (t_shape *) \
			&m->cylinders[((t_disc *)ray.shape - m->discs) / 2];
		m->shape_type = SHAPE_CYLINDER;
	}
	else
	{
		m->shape = (t_shape *)ray.shape;
		m->shape_type = ray.shape_type;
	}
}

static void	handle_world_click(t_minirt *m)
{
	t_ray	ray;

	m->click_x = 2. * m->mouse_x / m->img->width - 1;
	m->click_y = 2. * m->mouse_y / m->img->height - 1;
	m->click_x *= m->aspect_ratio;
	ray = cast_ray(m, m->mouse_x, m->mouse_y);
	m->moving_shape = (t_shape *)ray.shape;
	if (ray.shape_type == SHAPE_DISC)
		m->moving_shape = (t_shape *) \
			&m->cylinders[((t_disc *)ray.shape - m->discs) / 2];
	if (ray.shape != NULL)
		m->moving_shape_start = ray.shape->coords;
}

static void	handle_left_click(t_minirt *m)
{
	const t_click	c = click_type(\
		m, mlx_get_time(), m->mouse_x / CHAR_WIDTH, m->mouse_y / CHAR_HEIGHT);

	if (c == CLICK_DOUBLE)
		handle_double_click(m);
	else if (c == CLICK_CLOSE_MENU)
		m->shape_type = SHAPE_NO_SHAPE;
	else if (c == CLICK_OPEN_MENU)
		m->shape_type = SHAPE_GLOBAL_ATTRIBUTES;
	else if (c == CLICK_SHOW_LIGHTS)
	{
		m->show_lights = !m->show_lights;
		ft_memset(m->valid_pixel, false, m->valid_pixel_len);
	}
	else if (c == CLICK_SLIDER)
	{
		m->moving_slider = m->mouse_y / CHAR_HEIGHT - 1;
		edit_objects(m, m->mouse_x);
	}
	else if (c == CLICK_WORLD)
		handle_world_click(m);
}

void	mouse_hook(
	mouse_key_t button, action_t action, modifier_key_t mods, void *minirt)
{
	t_minirt *const	m = minirt;

	(void)mods;
	if (button == MLX_MOUSE_BUTTON_LEFT && action == MLX_PRESS)
	{
		handle_left_click(m);
	}
	if (button == MLX_MOUSE_BUTTON_RIGHT && (action == MLX_PRESS
			|| action == MLX_REPEAT))
		m->mouse_r_down = true;
	else
		m->mouse_r_down = false;
	if (button == MLX_MOUSE_BUTTON_LEFT && action == MLX_RELEASE)
	{
		m->moving_slider = false;
		m->moving_shape = NULL;
	}
}
