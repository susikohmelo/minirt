/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:06:04 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/13 16:19:55 by lfiestas         ###   ########.fr       */
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
    // double  t;
    // double  p;
    // t_vec3  dir;

    // dir = m->camera_orientation;
    // t = acos(dir.z) + dt;
    // if (sqrt(dir.x * dir.x + dir.y * dir.y) == 0)
    //     p = dp;
    // else if (dir.y >= 0)
    //     p = acos(dir.x / sqrt(dir.x * dir.x + dir.y * dir.y)) + dp;
    // else
    //     p = -acos(dir.x / sqrt(dir.x * dir.x + dir.y * dir.y)) + dp;

    // static double   t;
    // static double   p;

    // if (t + dt < 0)
    //     t = M_PI - (t + dt);
    // else if (t + dt > M_PI)
    //     t = (t + dt) - M_PI;
    // else
    //     t = (t + dt);

    // if (p + dp < 0)
    //     p = M_PI - (p + dp);
    // else if (p + dp > 2 * M_PI)
    //     p = (p + dp) - 2 * M_PI;
    // else
    //     p = (p + dp);

    // m->camera_orientation = vec3(sin(t) * cos(p), sin(t) * sin(p), cos(t));

    m->camera_orientation = vec3_inverse_lookat(vec3_rotatex(vec3(0,0,1), dp), m->camera_orientation);
    m->camera_orientation = vec3_inverse_lookat(vec3_rotatey(vec3(0,0,1), dt), m->camera_orientation);
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
            vec3_rotatey(dir, M_PI / 2), .5 * (1 - 2 * (key.key != MLX_KEY_A))));

    if (key.key == MLX_KEY_UP || key.key == MLX_KEY_DOWN)
        rotate_camera(m, 0, .125 * (1 - 2 * (key.key != MLX_KEY_UP)));
    if (key.key == MLX_KEY_LEFT || key.key == MLX_KEY_RIGHT)
        rotate_camera(m, .125 * (1 - 2 * (key.key != MLX_KEY_LEFT)), 0);

    // if (key.key == MLX_KEY_UP || key.key == MLX_KEY_DOWN)
    //     m->camera_orientation = vec3_normalize(vec3_add(dir, vec3_muls(vec3( \
    //         dir.z, dir.x, dir.y), .25)));

    // if (key.key == MLX_KEY_UP || key.key == MLX_KEY_DOWN)
    //     //m->camera_orientation = axis_rotation(dir, vec3(1,0,0),
    //     //m->camera_orientation = axis_rotation(dir, vec3(dir.z, dir.x, dir.y),
    //     //m->camera_orientation = axis_rotation(dir, vec3_rotatex(dir, M_PI / 2),
    //     m->camera_orientation = axis_rotation(dir, vec3(dir.z, dir.y, dir.x),
    //         (M_PI / 16) * (1 - 2 * (key.key != MLX_KEY_UP)));
    // if (key.key == MLX_KEY_LEFT || key.key == MLX_KEY_RIGHT)
    //     //m->camera_orientation = axis_rotation(dir, vec3(0,1,0),
    //     //m->camera_orientation = axis_rotation(dir, vec3(dir.y, dir.z, dir.x),
    //     //m->camera_orientation = axis_rotation(dir, vec3_rotatey(dir, M_PI / 2),
    //     m->camera_orientation = axis_rotation(dir, vec3(dir.x, dir.y, dir.z),
    //         (M_PI / 16) * (1 - 2 * (key.key != MLX_KEY_LEFT)));

    // if (key.key == MLX_KEY_UP || key.key == MLX_KEY_DOWN)
    //     m->camera_orientation = vec3_rotatex(dir, \
    //         (M_PI / 32) * (1 - 2 * (key.key != MLX_KEY_UP)));
    // if (key.key == MLX_KEY_LEFT || key.key == MLX_KEY_RIGHT)
    //     m->camera_orientation = vec3_rotatey(dir, \
    //         (M_PI / 32) * (1 - 2 * (key.key != MLX_KEY_LEFT)));

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
	t_minirt	*m;

	(void)mods;
	m = minirt;
	if (button == MLX_MOUSE_BUTTON_LEFT && action == MLX_PRESS)
	 	ft_memset(m->valid_pixel, false, sizeof m->valid_pixel);
}

void	cursor_hook(double x, double y, void *minirt)
{
	t_minirt	*m;

	m = minirt;
	m->mouse_x = x;
	m->mouse_y = y;
}
