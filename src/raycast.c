/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 15:21:40 by ljylhank          #+#    #+#             */
/*   Updated: 2025/01/31 21:08:41 by ljylhank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raycast.h"
#include "minirt.h"
#include <math.h>
#include <stdio.h>

static inline t_vec3	pix_to_scrspace(t_minirt *minirt, int x, int y)
{
	t_vec3	rval;

	rval.x = (2 * (x / (double) minirt->mlx->width) - 1) * minirt->aspect_ratio;
	rval.y = (2 * (y / (double) minirt->mlx->height) - 1);
	rval.z = 0;
	return (rval);
}

// TODO if fov is 0 this will segfault
// TODO fov has to be under 180
/*
	multiplying by 0.01745 converts degrees to radians
*/
static t_ray	init_ray(t_minirt *minirt, int x, int y)
{
	t_ray	new_ray;
	t_vec3	pos_screenspace;
	double	cmr_dist_from_scr;

	cmr_dist_from_scr = -(1 / tan(minirt->camera_field_of_view / 2 * 0.01745));
	new_ray.start = (t_vec3) { .x = 0, .y = 0, .z = cmr_dist_from_scr };
	pos_screenspace = pix_to_scrspace(minirt, x, y);
	new_ray.dir = vec3_sub(pos_screenspace, new_ray.start);
	return (new_ray);
}


// TODO if height of the window is 0 this will segfault
void	cast_rays(t_minirt *minirt)
{
	t_ray	ray;
	int		column;
	int		row;

	minirt->aspect_ratio = (double) minirt->mlx->width / (double) minirt->mlx->height;
	row = -1;
	while (++row < minirt->mlx->height)
	{
		column = -1;
		while (++column < minirt->mlx->width)
		{
			ray = init_ray(minirt, column, row);
			// TODO TEMP
			minirt->img->pixels[column + row * minirt->mlx->width] = 255;
			printf("%d %d - %f %f %f\n", row, column, ray.dir.x, ray.dir.y, ray.dir.z);
		}
	}
}

void	render_frame(void *rt_voidptr)
{
	t_minirt	*minirt;

	minirt = (t_minirt *) rt_voidptr;
	// TODO TEMP
	minirt->camera_coords.x = 0;
	minirt->camera_coords.y = 0;
	minirt->camera_coords.z = 0;
	minirt->camera_orientation.x = 0;
	minirt->camera_orientation.y = 0;
	minirt->camera_orientation.z = 1;
	minirt->camera_field_of_view = 90;
	// TODO ALSO TEMP DUH
	cast_rays(minirt);
	mlx_image_to_window(minirt->mlx, minirt->img, 200, 200);
}
