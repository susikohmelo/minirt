/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_uv_functions.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 16:58:10 by ljylhank          #+#    #+#             */
/*   Updated: 2025/02/06 22:35:01 by ljylhank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>
#include <fcntl.h>
#include <unistd.h>

mlx_image_t	*load_texture(t_minirt *m, char *filename)
{
	xpm_t		*xpm;
	mlx_image_t	*img;

	xpm = mlx_load_xpm42(filename);
	if (!xpm)
	{
		ft_putstr_fd("WARN: xpm42 could not be loaded, loading default\n", 2);
		xpm = mlx_load_xpm42("textures/default.xpm42");
		mrt_assert(m, xpm, "default xpm42 could not be loaded");
	}
	img = mlx_texture_to_image(m->mlx, &xpm->texture);
	mrt_assert(m, img, "converting xpm42 to image failed");
	mlx_delete_xpm42(xpm);
	return (img);
}

t_vec3	get_texture_from_uv(mlx_image_t *img, double u, double v)
{
	t_vec3	color;
	int		pos;
	int		x;
	int		y;

	x = img->width - round(u * (double) img->width);
	y = round(v * (double) img->height);
	pos = (y * img->width + x) * 4;
	color.r = ((double) img->pixels[pos + 0] / 255);
	color.g = ((double) img->pixels[pos + 1] / 255);
	color.b = ((double) img->pixels[pos + 2] / 255);
	return (color);
}
