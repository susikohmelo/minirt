/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 15:21:40 by ljylhank          #+#    #+#             */
/*   Updated: 2025/02/03 17:32:39 by ljylhank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raycast.h"
#include "minirt.h"
#include <math.h>
#include <stdio.h>

static inline t_vec3	pix_to_scrspace(t_minirt *minirt, double x, double y)
{
	t_vec3	rval;

	rval.x = (2 * ((x + 0.5) / (double) minirt->mlx->width) - 1) * minirt->aspect_ratio;
	rval.y = -(2 * ((y + 0.5) / (double) minirt->mlx->height) - 1);
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
	printf("dad %f\n", cmr_dist_from_scr);
	new_ray.start = (t_vec3) { .x = 0, .y = 0, .z = cmr_dist_from_scr };
	pos_screenspace = pix_to_scrspace(minirt, (double) x, (double) y);
	new_ray.dir = vec3_sub(pos_screenspace, new_ray.start);
	new_ray.len = 9999;
	return (new_ray);
}


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
			t_vec3 s_pos = (t_vec3) { 0.75, 0.75, 1 };
			t_vec3 lstart = vec3_sub(ray.start, s_pos);

			double a = vec3_length(ray.dir) * vec3_length(ray.dir);
			double b = 2 * vec3_dot(ray.dir, lstart);
			double c = vec3_length(lstart) * vec3_length(lstart) - 1 * 1;
			double discriminant = b * b - 4 * a * c;
			// TODO TEMP
			if (discriminant >= 0)
				mlx_put_pixel(minirt->img, column, row, 0XFF0000FF);
			//printf("x%d y%d: %f %f %f\n", column, row, ray.dir.x, ray.dir.y, ray.dir.z);
		}
	}
}

void	render_frame(void *rt_voidptr)
{
	t_minirt	*minirt;

	minirt = (t_minirt *) rt_voidptr;
	// TODO TEMP
	minirt->camera_field_of_view = 90;
	cast_rays(minirt);
	mlx_image_to_window(minirt->mlx, minirt->img, 0, 0);
}
