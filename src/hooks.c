/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:06:04 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/14 15:24:54 by lfiestas         ###   ########.fr       */
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

static
void rotate_camera(t_minirt *m, double dt, double dp)
{
    m->camera_orientation = vec3_inverse_lookat(vec3_rotatex(vec3(0,0,1), dp), m->camera_orientation);
    m->camera_orientation = vec3_inverse_lookat(vec3_rotatey(vec3(0,0,1), dt), m->camera_orientation);
}

static t_vec3	perpendiculary(t_vec3 v)
{
	t_vec3	result;

	result = vec3_cross(v, vec3(0, 1, 0));
	return (vec3_normalize(result));
}

void	key_hook(mlx_key_data_t key, void *minirt)
{
    t_minirt    *m;
    t_vec3      dir;

    m = minirt;
    dir = m->camera_orientation;

	if (key.key == MLX_KEY_ESCAPE)
		mrt_exit(minirt, EXIT_SUCCESS);

	if (key.key == MLX_KEY_W || key.key == MLX_KEY_S)
		m->camera_coords = vec3_add(m->camera_coords, vec3_muls( \
			m->camera_orientation, .5 * (1 - 2 * (key.key != MLX_KEY_W))));
	if (key.key == MLX_KEY_A || key.key == MLX_KEY_D)
		m->camera_coords = vec3_add(m->camera_coords, vec3_muls( \
			perpendiculary(dir), .5 * (1 - 2 * (key.key != MLX_KEY_A))));
	if (key.key == MLX_KEY_PAGE_UP || key.key == MLX_KEY_PAGE_DOWN)
		m->camera_coords.y += .5 * (1 - 2 * (key.key != MLX_KEY_PAGE_UP));

    if (key.key == MLX_KEY_UP || key.key == MLX_KEY_DOWN)
        rotate_camera(m, 0, .125 * (1 - 2 * (key.key != MLX_KEY_UP)));
    if (key.key == MLX_KEY_LEFT || key.key == MLX_KEY_RIGHT)
        rotate_camera(m, .125 * (1 - 2 * (key.key != MLX_KEY_LEFT)), 0);

    ft_memset(m->valid_pixel, false, sizeof m->valid_pixel);
}

void	resize_hook(int w, int h, void *minirt)
{
	t_minirt	*m;

	m = minirt;
	mrt_assert(m, mlx_resize_image(m->img, w, h), "mlx_resize_image() failed");
	ft_memset(m->valid_pixel, false, sizeof m->valid_pixel);
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
}

void	cursor_hook(double x, double y, void *minirt)
{
	t_minirt	*m;

	m = minirt;
	if (m->double_clicked)
		return ;
	m->mouse_x = x;
	m->mouse_y = y;
}
