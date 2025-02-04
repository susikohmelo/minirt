/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_frame.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 21:06:44 by ljylhank          #+#    #+#             */
/*   Updated: 2025/02/04 23:02:40 by ljylhank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	render_frame(void *rt_voidptr)
{
	t_minirt	*minirt;

	minirt = (t_minirt *) rt_voidptr;
	cast_rays(minirt);
	mlx_image_to_window(minirt->mlx, minirt->img, 0, 0);
}
