/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:06:04 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/19 17:48:03 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <stdlib.h>
#include <math.h>

static void	handle_movement(t_minirt *m, mlx_key_data_t key)
{
	t_vec3			cam_orient_no_y;
	const double	d = MOVE_DISTANCE;
	t_vec3			dir;

	dir = m->camera_orientation;
	cam_orient_no_y = vec3(dir.x, 0, dir.z);
	cam_orient_no_y = vec3_normalize(cam_orient_no_y);
	if (key.key == MLX_KEY_W || key.key == MLX_KEY_S)
		m->camera_coords = vec3_add(m->camera_coords, vec3_muls(\
			cam_orient_no_y, d * (1 - 2 * (key.key != MLX_KEY_W))));
	if (key.key == MLX_KEY_A || key.key == MLX_KEY_D)
		m->camera_coords = vec3_add(m->camera_coords, vec3_muls(\
			perpendiculary(dir), d * (1 - 2 * (key.key != MLX_KEY_A))));
	if (key.key == MLX_KEY_SPACE || key.key == MLX_KEY_LEFT_SHIFT)
		m->camera_coords.y += d * (1 - 2 * (key.key != MLX_KEY_SPACE));
}

void	key_hook(mlx_key_data_t key, void *minirt)
{
	t_minirt		*m;

	if (key.action == MLX_RELEASE)
		return ;
	m = minirt;
	handle_movement(m, key);
	if (key.key == MLX_KEY_ESCAPE)
		mrt_exit(m, EXIT_SUCCESS);
	if (key.key == MLX_KEY_1 || key.key == MLX_KEY_2)
		m->max_ray_bounces = fmax(fmin(\
			m->max_ray_bounces + (1 - 2 * (key.key != MLX_KEY_2)), 1024), 0);
	if (key.key == MLX_KEY_UP || key.key == MLX_KEY_DOWN)
		m->valid_pixel_len = fmax(fmin(m->valid_pixel_len
					* (2 - 1.5 * (key.key == MLX_KEY_UP)), 2048), 2);
	if (key.key == MLX_KEY_3)
		m->disable_skybox = m->disable_skybox != 1;
	ft_memset(m->valid_pixel, false, m->valid_pixel_len);
}

void	resize_hook(int w, int h, void *minirt)
{
	t_minirt	*m;

	m = minirt;
	mrt_assert(m, mlx_resize_image(m->img, w, h), "mlx_resize_image() failed");
	m->resizing = true;
	ft_memset(m->valid_pixel, false, m->valid_pixel_len);
}

void	scroll_hook(double x_delta, double y_delta, void *minirt)
{
	t_minirt	*m;
	t_cylinder	*c;

	m = minirt;
	ft_memset(m->valid_pixel, false, m->valid_pixel_len);
	if (m->moving_shape)
	{
		m->moving_shape->coords = vec3_add(\
			m->moving_shape->coords, \
			vec3_muls(m->camera_orientation, y_delta * SCROLL_SENSITIVITY));
		m->moving_shape_start = vec3_add(\
			m->moving_shape_start, \
			vec3_muls(m->camera_orientation, y_delta * SCROLL_SENSITIVITY));
		c = (t_cylinder *)m->moving_shape;
		if (m->cylinders <= c && c < m->cylinders + m->cylinders_length)
		{
			m->discs[2 * (c - m->cylinders) + 0].coords = vec3_add(\
				c->coords, vec3_muls(c->axis, c->height / 2));
			m->discs[2 * (c - m->cylinders) + 1].coords = vec3_sub(\
				c->coords, vec3_muls(c->axis, c->height / 2));
		}
	}
	m->camera_field_of_view -= !m->moving_shape * (y_delta + x_delta);
	m->camera_field_of_view = fmax(fmin(m->camera_field_of_view, 180), 0);
}

void	cursor_hook(double x, double y, void *minirt)
{
	t_minirt *const	m = minirt;
	t_vec3			new_rot;
	t_vec3			mouse_move_dir;

	mouse_move_dir = vec3(m->mouse_x - x, m->mouse_y - y, 0);
	if (m->mouse_r_down && (mouse_move_dir.x != 0 || mouse_move_dir.y != 0))
	{
		mouse_move_dir = vec3_muls(mouse_move_dir, MOUSE_SENSITIVITY / 100);
		new_rot = vec3(0, 0, 1);
		new_rot.x = new_rot.x + -mouse_move_dir.x;
		if (m->camera_orientation.y + mouse_move_dir.y <= 1
			&& m->camera_orientation.y + mouse_move_dir.y >= -1)
			new_rot.y = new_rot.y + mouse_move_dir.y;
		new_rot = vec3_normalize(new_rot);
		m->camera_orientation = vec3_inverse_lookat(\
			new_rot, m->camera_orientation);
		ft_memset(m->valid_pixel, false, m->valid_pixel_len);
	}
	if (m->moving_slider)
		edit_objects(m, x);
	if (m->moving_shape && (mouse_move_dir.x != 0 || mouse_move_dir.y != 0))
		move_shape(m, x, y);
	m->mouse_x = x;
	m->mouse_y = y;
}
