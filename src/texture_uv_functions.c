/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_uv_functions.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 16:58:10 by ljylhank          #+#    #+#             */
/*   Updated: 2025/02/06 16:20:36 by ljylhank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>

static int set_rgba(int r, int g, int b, int a)
{
    return (r << 24 | g << 16 | b << 8 | a);
}

mlx_image_t	*load_texture(t_minirt *m, char *filename)
{
	xpm_t		*xpm;
	mlx_image_t	*img;
	bool		no_filename;
	int			i;

	i = 0;
	no_filename = true;
	while (filename && filename[i])
		if (!ft_isspace(filename[i++]))
			no_filename = false;
	if (no_filename)
		return (NULL);
	else
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

int get_texture_from_uv(mlx_image_t *img, double u, double v)
{
	int			pixel;
	int			x;
	int			y;
	int			pos;

	x = img->width - round(u * (double) img->width);
	y = round(v * (double) img->height);
	pos = (y * img->width + x) * 4;
	return (set_rgba(img->pixels[pos], img->pixels[pos + 1], img->pixels[pos + 2], img->pixels[pos + 3]));
}
