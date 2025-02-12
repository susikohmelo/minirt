/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:06:04 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/12 14:11:21 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <stdlib.h>

void	key_hook(mlx_key_data_t key, void *minirt)
{
	if (key.key == MLX_KEY_ESCAPE)
		mrt_exit(minirt, EXIT_SUCCESS);
}

void	resize_hook(int w, int h, void *minirt)
{
	t_minirt	*m;

	m = minirt;
	mrt_assert(m, mlx_resize_image(m->img, w, h), "mlx_resize_image() failed");
	ft_memset(m->valid_pixel, false, sizeof m->valid_pixel);
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
