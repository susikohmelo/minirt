/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_skybox_color.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 04:22:45 by ljylhank          #+#    #+#             */
/*   Updated: 2025/02/16 01:31:27 by ljylhank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>

static inline int	check_up_back_down(t_vec3 dir, double *l)
{
	t_vec3	pos;

	pos = vec3(10,10,10);
	*l = -1 / vec3_dot(dir, vec3(0,-1,0));
	if (*l >= 0)
		pos = vec3_muls(dir, *l);
	if (pos.y > 0 && pos.x <= 1 && pos.x >= -1 && pos.z <= 1 && pos.z >= -1)
		return (3);
	*l = -1 / vec3_dot(dir, vec3(0,0,1));
	if (*l >= 0)
		pos = vec3_muls(dir, *l);
	if (pos.z < 0 && pos.x <= 1 && pos.x >= -1 && pos.y <= 1 && pos.y >= -1)
		return (4);
	*l = -1 / vec3_dot(dir, vec3(0,1,0));
	if (*l >= 0)
		pos = vec3_muls(dir, *l);
	if (pos.y < 0 && pos.x <= 1 && pos.x >= -1 && pos.z <= 1 && pos.z >= -1)
		return (5);
	return (-1);
}

static inline int	check_left_front_right(t_vec3 dir, double *l)
{
	t_vec3	pos;

	pos = vec3(10,10,10);
	*l = -1 / vec3_dot(dir, vec3(1,0,0));
	if (*l >= 0)
		pos = vec3_muls(dir, *l);
	if (pos.x < 0 && pos.z <= 1 && pos.z >= -1 && pos.y <= 1 && pos.y >= -1)
		return (0);
	*l = -1 / vec3_dot(dir, vec3(0,0,-1));
	if (*l >= 0)
		pos = vec3_muls(dir, *l);
	if (pos.z > 0 && pos.x <= 1 && pos.x >= -1 && pos.y <= 1 && pos.y >= -1)
		return (1);
	*l = -1 / vec3_dot(dir, vec3(-1,0,0));
	if (*l >= 0)
		pos = vec3_muls(dir, *l);
	if (pos.x > 0 && pos.z <= 1 && pos.z >= -1 && pos.y <= 1 && pos.y >= -1)
		return (2);
	return (-1);
}

static mlx_image_t	*choose_skybox_img(t_minirt *m, t_vec3 dir,
						t_vec3 *normal, double *ray_len)
{
	int			plane_normal;

	plane_normal = check_left_front_right(dir, ray_len);
	if (plane_normal == -1)
		plane_normal = check_up_back_down(dir, ray_len);
	if (plane_normal == -1)
		return (NULL);
	if (plane_normal == 0)
		*normal = vec3(1,0,0);
	else if (plane_normal == 1)
		*normal = vec3(0,0,-1);
	else if (plane_normal == 2)
		*normal = vec3(-1,0,0);
	else if (plane_normal == 3)
		*normal = vec3(0,-1,0);
	else if (plane_normal == 4)
		*normal = vec3(0,0,1);
	else if (plane_normal == 5)
		*normal = vec3(0,1,0);
	return (m->skybox.sky_array[plane_normal]);
}

t_vec3	get_skybox_color(t_minirt *m, t_vec3 dir, double blur)
{
	double		ray_len;
	mlx_image_t	*sky_img;
	t_vec3		normal;
	t_vec2		uv;

	if (m->disable_skybox)
		return ((t_vec3){});
	sky_img = choose_skybox_img(m, dir, &normal, &ray_len);
	if (!sky_img)
		return (vec3(0,1,0));
	dir = vec3_muls(vec3_lookat(dir, normal), ray_len - 0.01);
	uv.x = fmax(fmax(fmin(dir.x, 1), -1) * 0.5 + 0.5, 0);
	uv.y = fmax(fmax(fmin(dir.y, 1), -1) * 0.5 + 0.5, 0);
	return (get_texture_from_uv(sky_img, uv.x, uv.y, blur));
}
