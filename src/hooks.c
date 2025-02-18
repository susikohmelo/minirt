/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:06:04 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/18 11:03:10 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <stdlib.h>
#include <math.h>

//static // TODO maybe move this or something
t_vec3   axis_rotation(t_vec3 v, t_vec3 axis, double r)
{
    const double    m[3][3] = {{
            axis.x * axis.x * (1 - cos(r)) + cos(r),
            axis.x * axis.y * (1 - cos(r)) - axis.z * sin(r),
            axis.x * axis.z * (1 - cos(r)) + axis.y * sin(r)
        },{
            axis.x * axis.y * (1 - cos(r)) + axis.z * sin(r),
            axis.y * axis.y * (1 - cos(r)) + cos(r),
            axis.y * axis.z * (1 - cos(r)) - axis.x * sin(r)
        },{
            axis.x * axis.z * (1 - cos(r)) - axis.y * sin(r),
            axis.y * axis.z * (1 - cos(r)) + axis.x * sin(r),
            axis.z * axis.z * (1 - cos(r)) + cos(r)
        }
    };
    return (mat3_vec3(m, v));
}

void	redraw(t_minirt *m, bool flush_black)
{
	size_t	row;
	size_t	column;

	if (flush_black)
	{
		ft_memset(m->img->pixels, 0, m->img->width * m->img->height * 4);
		row = (size_t) - 1;
		while (++row < m->img->height)
		{
			column = (size_t) - 1;
			while (++column < m->img->width)
				m->img->pixels[4 * (row * m->img->width + column) + 3] = 255;
		}
	}
	m->valid_pixel_x = 0;
	m->valid_pixel_y = 0;
}

t_vec3	perpendiculary(t_vec3 v)
{
	t_vec3	result;

	result = vec3_cross(v, vec3(0, 1, 0));
	return (vec3_normalize(result));
}

t_vec3	perpendicularx(t_vec3 v)
{
	t_vec3	result;

	result = vec3_cross(v, vec3(1, 0, 0));
	return (vec3_normalize(result));
}

void	key_hook(mlx_key_data_t key, void *minirt)
{
    t_minirt		*m;
    t_vec3			dir;
    t_vec3			cam_orient_no_y;
	const double	d = MOVE_DISTANCE;

	if (key.action == MLX_RELEASE)
		return ;
    m = minirt;
    dir = m->camera_orientation;
	if (key.key == MLX_KEY_ESCAPE)
		mrt_exit(minirt, EXIT_SUCCESS);
	cam_orient_no_y = vec3(m->camera_orientation.x, 0, m->camera_orientation.z);
	cam_orient_no_y = vec3_normalize(cam_orient_no_y);
	if (key.key == MLX_KEY_W || key.key == MLX_KEY_S)
		m->camera_coords = vec3_add(m->camera_coords, vec3_muls( \
			cam_orient_no_y, d * (1 - 2 * (key.key != MLX_KEY_W))));
	if (key.key == MLX_KEY_A || key.key == MLX_KEY_D)
		m->camera_coords = vec3_add(m->camera_coords, vec3_muls( \
			perpendiculary(dir), d * (1 - 2 * (key.key != MLX_KEY_A))));
	if (key.key == MLX_KEY_SPACE || key.key == MLX_KEY_LEFT_SHIFT)
		m->camera_coords.y += d * (1 - 2 * (key.key != MLX_KEY_SPACE));
	if (key.key == MLX_KEY_1 || key.key == MLX_KEY_2)
		m->max_ray_bounces = fmax(fmin(m->max_ray_bounces
			+ (1 - 2 * (key.key != MLX_KEY_2)), 1024), 0);
	if (key.key == MLX_KEY_UP || key.key == MLX_KEY_DOWN)
		m->valid_pixel_len = fmax(fmin((int) m->valid_pixel_len
					+ (1 - 2 * (key.key != MLX_KEY_DOWN)), 1024), 0);
	if (key.key == MLX_KEY_3)
		m->disable_skybox = m->disable_skybox != 1;
	redraw(m, true);
}

void	resize_hook(int w, int h, void *minirt)
{
	t_minirt	*m;

	m = minirt;
	mrt_assert(m, mlx_resize_image(m->img, w, h), "mlx_resize_image() failed");
	redraw(m, true);
	m->resizing = true;
}

void	mouse_hook(
	mouse_key_t button, action_t action, modifier_key_t mods, void *minirt)
{
	t_minirt		*const m = minirt;
	static double	last_click_time;
	double			click_time;
	bool			double_clicked;
	const bool		clicked_slider = m->shape_type != SHAPE_NO_SHAPE
		&& 0 <= m->mouse_x && m->mouse_x <= LINE_LENGTH * CHAR_WIDTH
		&& 2 * CHAR_HEIGHT <= m->mouse_y
		&& m->mouse_y <= (int)m->gui_line * CHAR_HEIGHT;
	const bool		clicked_close_menu = (LINE_LENGTH - 1) * CHAR_WIDTH <= m->mouse_x
		&& m->mouse_x <= LINE_LENGTH * CHAR_WIDTH
		&& 0 <= m->mouse_y && m->mouse_y <= CHAR_HEIGHT
		&& m->shape_type != SHAPE_NO_SHAPE;
	const bool		clicked_open_menu = 0 <= m->mouse_x && m->mouse_x <= CHAR_WIDTH
		&& 0 <= m->mouse_y && m->mouse_y <= CHAR_HEIGHT
		&& m->shape_type == SHAPE_NO_SHAPE;
	const bool		clicked_show_lights = (LINE_LENGTH - 3) * CHAR_WIDTH <= m->mouse_x
		&& m->mouse_x <= LINE_LENGTH * CHAR_WIDTH
		&& (int)(m->gui_line - 1) * CHAR_HEIGHT <= m->mouse_y
		&& m->mouse_y <= (int)m->gui_line * CHAR_HEIGHT
		&& m->shape_type == SHAPE_GLOBAL_ATTRIBUTES;

	(void)mods;
	if (button == MLX_MOUSE_BUTTON_LEFT && action == MLX_PRESS)
	{
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
			redraw(m, false);
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
			redraw(m, false);
		}
		last_click_time = click_time;
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

void	scroll_hook(double x_delta, double y_delta, void *minirt)
{
	t_minirt	*m;

	m = minirt;
	if (m->moving_shape)
	{
		m->moving_shape->coords = vec3_add(m->moving_shape->coords, vec3_muls(m->camera_orientation, y_delta * SCROLL_SENSITIVITY));
		m->moving_shape_start = vec3_add(m->moving_shape_start, vec3_muls(m->camera_orientation, y_delta * SCROLL_SENSITIVITY));
		redraw(m, true);
		return ;
	}
	y_delta += x_delta;
	m->camera_field_of_view -= y_delta;
	m->camera_field_of_view = fmax(fmin(m->camera_field_of_view, 179), 1);
	redraw(m, true);
}

// TODO vec3_rotatexy is not used anymore, at least in hooks.c
void	cursor_hook(double x, double y, void *minirt)
{
	t_minirt		*m;
	t_vec3			new_rot;
	t_vec3			mouse_move_dir;
	bool			mouse_moved_this_frame;

	m = minirt;
	mouse_move_dir = vec3(m->mouse_x - x, m->mouse_y - y, 0);
	mouse_moved_this_frame = mouse_move_dir.x != 0 || mouse_move_dir.y != 0;
	if (m->mouse_r_down && mouse_moved_this_frame)
	{
		mouse_move_dir = vec3_muls(mouse_move_dir, MOUSE_SENSITIVITY / 100);
		new_rot = vec3(0,0,1);
		new_rot.x = new_rot.x + -mouse_move_dir.x;
		if (m->camera_orientation.y + mouse_move_dir.y <= 1
				&& m->camera_orientation.y + mouse_move_dir.y >= -1)
			new_rot.y = new_rot.y + mouse_move_dir.y;
		new_rot = vec3_normalize(new_rot);
		m->camera_orientation = vec3_inverse_lookat(new_rot, m->camera_orientation);
		redraw(m, true);
	}
	if (m->moving_slider)
		edit_objects(m, x);
	if (m->moving_shape && mouse_moved_this_frame)
		move_shape(m, x, y);
	m->mouse_x = x;
	m->mouse_y = y;
}
