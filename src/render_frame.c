/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_frame.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 21:06:44 by ljylhank          #+#    #+#             */
/*   Updated: 2025/02/12 13:35:38 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	render_frame(void *minirt)
{
	t_minirt	*m;

	m = minirt;
	cast_rays(minirt);
	m->valid_pixel[m->valid_pixel_i] = true;
	m->valid_pixel_i = (m->valid_pixel_i + 5) & (sizeof m->valid_pixel - 1);
	mrt_assert(m, mlx_image_to_window(m->mlx, m->img, 0, 0), \
		"mlx_image_to_window() failed");
}
