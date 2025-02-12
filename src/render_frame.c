/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_frame.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 21:06:44 by ljylhank          #+#    #+#             */
/*   Updated: 2025/02/12 15:27:15 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	render_frame(void *minirt)
{
	const char	ones[] = "\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1";
	t_minirt	*m;

	m = minirt;
	cast_rays(minirt);
	m->valid_pixel[m->valid_pixel_i] = true;
	m->valid_pixel_i = (m->valid_pixel_i + 5) & (sizeof m->valid_pixel - 1);
	if (ft_memcmp(m->valid_pixel, ones, sizeof m->valid_pixel) == 0)
		m->resizing = false;
	mrt_assert(m, mlx_image_to_window(m->mlx, m->img, 0, 0), \
		"mlx_image_to_window() failed");
}
