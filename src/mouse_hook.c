/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_hook.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:15:06 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/19 16:17:31 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

// TODO move to header
typedef enum	e_click
{
	CLICK_NO_CLICK,
	CLICK_DOUBLE,
	CLICK_SLIDER,
	CLICK_CLOSE_MENU,
	CLICK_OPEN_MENU,
	CLICK_SHOW_LIGHTS,
	CLICK_WORLD,
}	t_click;

// static t_click	click_type(t_minirt *m)
// {
//
// }

static void	handle_left_click(t_minirt *m)
{
	static double	last_click_time;
	double			click_time;
	bool			double_clicked;
	const bool		clicked_menu = 0 <= m->mouse_x && 0 <= m->mouse_y
		&& m->mouse_x <= LINE_LENGTH * CHAR_WIDTH
		&& m->mouse_y <= CHAR_HEIGHT * (int)m->gui_line;
	const bool		clicked_slider = m->shape_type != SHAPE_NO_SHAPE
		&& clicked_menu && 2 * CHAR_HEIGHT <= m->mouse_y;
	const bool		clicked_close_menu = (LINE_LENGTH - 1) * CHAR_WIDTH <= m->mouse_x
		&& clicked_menu && m->shape_type != SHAPE_NO_SHAPE;
	const bool		clicked_open_menu = clicked_menu
		&& m->mouse_x <= CHAR_WIDTH && m->mouse_y <= CHAR_HEIGHT
		&& m->shape_type == SHAPE_NO_SHAPE;
	const bool		clicked_show_lights = clicked_menu
		&& (LINE_LENGTH - 3) * CHAR_WIDTH <= m->mouse_x
		&& (int)(m->gui_line - 1) * CHAR_HEIGHT <= m->mouse_y
		&& m->shape_type == SHAPE_GLOBAL_ATTRIBUTES;

	click_time = mlx_get_time();
	double_clicked = click_time - last_click_time < .2 && !clicked_slider
		&& 0 <= m->mouse_x && m->mouse_x < (int)m->img->width
		&& 0 <= m->mouse_y && m->mouse_y < (int)m->img->height;
	if (double_clicked)
	{
		t_ray ray = cast_ray(m, m->mouse_x, m->mouse_y);
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
	else if (clicked_close_menu)
		m->shape_type = SHAPE_NO_SHAPE;
	else if (clicked_open_menu)
		m->shape_type = SHAPE_GLOBAL_ATTRIBUTES;
	else if (clicked_show_lights)
	{
		m->show_lights = !m->show_lights;
		ft_memset(m->valid_pixel, false, m->valid_pixel_len);
	}
	else if (clicked_slider)
	{
		m->moving_slider = m->mouse_y / CHAR_HEIGHT - 1;
		edit_objects(m, m->mouse_x);
	}
	else // clicked world
	{
		m->click_x = 2. * m->mouse_x / m->img->width - 1;
		m->click_y = 2. * m->mouse_y / m->img->height - 1;
		m->click_x *= m->aspect_ratio;
		t_ray ray = cast_ray(m, m->mouse_x, m->mouse_y);
		m->moving_shape = (t_shape *)ray.shape;
		if (ray.shape_type == SHAPE_DISC)
			m->moving_shape = (t_shape *) \
				&m->cylinders[((t_disc *)ray.shape - m->discs) / 2];
		if (ray.shape != NULL)
			m->moving_shape_start = ray.shape->coords;
	}
	last_click_time = click_time;
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

