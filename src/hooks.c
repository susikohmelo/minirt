/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:06:04 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/06 13:55:32 by lfiestas         ###   ########.fr       */
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
	(void)m;
	(void)w;
	(void)h;
}

void	mouse_hook(
	mouse_key_t button, action_t action, modifier_key_t mods, void *minirt)
{
	(void)mods;
	if (button == MLX_MOUSE_BUTTON_LEFT && action == MLX_PRESS)
		render_frame(minirt);
}

void	cursor_hook(double x, double y, void *minirt)
{
	t_minirt	*m;

	m = minirt;
	m->mouse_x = x;
	m->mouse_y = y;
}
