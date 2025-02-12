/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_textures.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 00:06:31 by ljylhank          #+#    #+#             */
/*   Updated: 2025/02/12 14:21:43 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static inline void	free_planes(t_minirt *m)
{
	int	len;

	len = (int) m->planes_length;
	while(len--)
	{
		if((*(m->planes + len)).texture)
			mlx_delete_image(m->mlx, (*(m->planes + len)).texture);
		if((*(m->planes + len)).normal_map)
			mlx_delete_image(m->mlx, (*(m->planes + len)).normal_map);
		if((*(m->planes + len)).roughness_map)
			mlx_delete_image(m->mlx, (*(m->planes + len)).roughness_map);
	}
}

static inline void	free_spheres(t_minirt *m)
{
	int	len;

	len = (int)	m->spheres_length;
	while(len--)
	{
		if((*(m->spheres + len)).texture)
			mlx_delete_image(m->mlx, (*(m->spheres + len)).texture);
		if((*(m->spheres + len)).normal_map)
			mlx_delete_image(m->mlx, (*(m->spheres + len)).normal_map);
		if((*(m->spheres + len)).roughness_map)
			mlx_delete_image(m->mlx, (*(m->spheres + len)).roughness_map);
	}
}

static inline void	free_cylinders(t_minirt *m)
{
	int	len;

	len = (int) m->cylinders_length;
	while(len--)
	{
		if((*(m->cylinders + len)).texture)
			mlx_delete_image(m->mlx, (*(m->cylinders + len)).texture);
		if((*(m->cylinders + len)).normal_map)
			mlx_delete_image(m->mlx, (*(m->cylinders + len)).normal_map);
		if((*(m->cylinders + len)).roughness_map)
			mlx_delete_image(m->mlx, (*(m->cylinders + len)).roughness_map);
	}
}

static inline void	free_discs(t_minirt *m)
{
	int	len;

	len = (int) m->discs_length;
	while (len--)
	{
		if((*(m->discs + len)).texture)
			mlx_delete_image(m->mlx, (*(m->discs + len)).texture);
		if((*(m->discs + len)).normal_map)
			mlx_delete_image(m->mlx, (*(m->discs + len)).normal_map);
		if((*(m->discs + len)).roughness_map)
			mlx_delete_image(m->mlx, (*(m->discs + len)).roughness_map);
	}
}

void	free_textures(t_minirt *m)
{
	free_planes(m);
	free_spheres(m);
	free_cylinders(m);
	free_discs(m);
}
