/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:06:04 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/15 01:41:46 by ljylhank         ###   ########.fr       */
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

void	flush_image_black(mlx_image_t *img)
{
	size_t	row;
	size_t	column;

	ft_memset(img->pixels, 0, img->width * img->height * sizeof(int32_t));
	row = (size_t) - 1;
	while (++row < img->height)
	{
		column = (size_t) - 1;
		while (++column < img->width)
			img->pixels[4 * (row * img->width + column) + 3] = 255;
	}
}

static t_vec3	perpendiculary(t_vec3 v)
{
	t_vec3	result;

	result = vec3_cross(v, vec3(0, 1, 0));
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
    ft_memset(m->valid_pixel, false, sizeof m->valid_pixel);
	flush_image_black(m->img);
	m->valid_pixel_i = 0;
}

void	resize_hook(int w, int h, void *minirt)
{
	t_minirt	*m;

	m = minirt;
	mrt_assert(m, mlx_resize_image(m->img, w, h), "mlx_resize_image() failed");
	ft_memset(m->valid_pixel, false, sizeof m->valid_pixel);
	flush_image_black(m->img);
	m->valid_pixel_i = 0;
	m->resizing = true;
}

void	mouse_hook(
	mouse_key_t button, action_t action, modifier_key_t mods, void *minirt)
{
	t_minirt		*m;
	static double	last_click_time;
	double			click_time;

	(void)mods;
	m = minirt;
	if (button == MLX_MOUSE_BUTTON_LEFT && action == MLX_PRESS)
	{
		click_time = mlx_get_time();
		if (click_time - last_click_time < .2)
		{
			m->double_clicked = true;
			ft_memset(m->valid_pixel, false, sizeof m->valid_pixel);
			flush_image_black(m->img);
			m->valid_pixel_i = 0;
		}
		else
		{
			if ((LINE_LENGTH - 1) * CHAR_WIDTH <= m->mouse_x
				&& m->mouse_x <= LINE_LENGTH * CHAR_WIDTH
				&& 0 <= m->mouse_y && m->mouse_y <= CHAR_HEIGHT
				&& m->shape_type != SHAPE_NO_SHAPE)
				m->shape_type = SHAPE_NO_SHAPE;
			else if (0 <= m->mouse_x && m->mouse_x <= CHAR_WIDTH
				&& 0 <= m->mouse_y && m->mouse_y <= CHAR_HEIGHT
				&& m->shape_type == SHAPE_NO_SHAPE)
				m->shape_type = SHAPE_GLOBAL_ATTRIBUTES;
		}
		last_click_time = click_time;
	}
	if (button == MLX_MOUSE_BUTTON_RIGHT && (action == MLX_PRESS
			|| action == MLX_REPEAT))
		m->mouse_r_down = true;
	else
		m->mouse_r_down = false;
}

void	scroll_hook(double x_delta, double y_delta, void *minirt)
{
	t_minirt	*m;

	m = minirt;
	y_delta += x_delta;
	m->camera_field_of_view -= y_delta;
	m->camera_field_of_view = fmax(fmin(m->camera_field_of_view, 179), 1);
	ft_memset(m->valid_pixel, false, sizeof m->valid_pixel);
	flush_image_black(m->img);
	m->valid_pixel_i = 0;
}

// TODO vec3_rotatexy is not used anymore, at least in hooks.c
void	cursor_hook(double x, double y, void *minirt)
{
	t_minirt	*m;
	t_vec3		new_rot;
	t_vec3		mouse_move_dir;

	m = minirt;
	mouse_move_dir = vec3(m->mouse_x - x, m->mouse_y - y, 0);
	if (m->mouse_r_down && (mouse_move_dir.x != 0 || mouse_move_dir.y != 0))
	{
		mouse_move_dir = vec3_muls(mouse_move_dir, MOUSE_SENSITIVITY / 100);
		new_rot = vec3(0,0,1);
		new_rot.x = new_rot.x + -mouse_move_dir.x;
		if (m->camera_orientation.y + mouse_move_dir.y <= 1
				&& m->camera_orientation.y + mouse_move_dir.y >= -1)
			new_rot.y = new_rot.y + mouse_move_dir.y;
		new_rot = vec3_normalize(new_rot);
		m->camera_orientation = vec3_inverse_lookat(new_rot, m->camera_orientation);
		ft_memset(m->valid_pixel, false, sizeof m->valid_pixel);
		flush_image_black(m->img);
		m->valid_pixel_i = 0;
	}
	m->mouse_x = x;
	m->mouse_y = y;
}
