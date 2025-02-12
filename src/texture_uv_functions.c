/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_uv_functions.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 16:58:10 by ljylhank          #+#    #+#             */
/*   Updated: 2025/02/12 21:36:05 by ljylhank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>
#include <fcntl.h>
#include <unistd.h>

mlx_image_t	*load_texture(t_minirt *m, char *filename, int texture_type)
{
	xpm_t		*xpm;
	mlx_image_t	*img;

	if (!filename)
		return (NULL);
	xpm = mlx_load_xpm42(filename);
	if (!xpm)
	{
		if (texture_type != ALBEDO)
			return (NULL);
		ft_putstr_fd("Warning\nxpm42 couldn't be loaded, loading default\n", 2);
		xpm = mlx_load_xpm42("textures/default.xpm42");
		if (!mrt_assert(m, xpm, "default xpm42 could not be loaded"))
			return (NULL);
	}
	img = mlx_texture_to_image(m->mlx, &xpm->texture);
	if (!mrt_assert(m, img, "converting xpm42 to image failed"))
		return (NULL);
	mlx_delete_xpm42(xpm);
	return (img);
}

static inline t_vec3	get_clr(mlx_image_t *img, int pos)
{
	t_vec3	color;

	color.r = ((double) img->pixels[pos + 0] / 255);
	color.g = ((double) img->pixels[pos + 1] / 255);
	color.b = ((double) img->pixels[pos + 2] / 255);
	return (color);
}

static t_vec3	get_nearby_pixels(mlx_image_t *img, int targ_x, int targ_y)
{
	t_vec3	clr;
	int		row;
	int		column;
	int		counter;
	int		xy[2];

	clr = (t_vec3){};
	counter = 0;
	row = -4;
	while (++row <= 3)
	{
		column = -4;
		while (++column <= 3)
		{
			xy[0] = targ_x + column * 6;
			xy[1] = targ_y + row * 6;
			if (xy[0] < 0 || xy[1] < 0 || xy[0] > (int) img->width - 1
				|| xy[1] > (int) img->height - 1)
				continue ;
			++counter;
			clr = vec3_add(clr, get_clr(img, (xy[1] * img->width + xy[0]) * 4));
		}
	}
	return (vec3_divs(clr, (double) counter));
}

static inline void	mix_colors(t_vec3 *c1, t_vec3 c2, double amount)
{
	c1->x = c1->x * (1 - amount) + c2.x * amount;
	c1->y = c1->y * (1 - amount) + c2.y * amount;
	c1->z = c1->z * (1 - amount) + c2.z * amount;
}

t_vec3	get_texture_from_uv(mlx_image_t *img, double u, double v, double blur)
{
	t_vec3	color;
	int		pos;
	int		x;
	int		y;

	v = 1 - v;
	x = img->width - round(u * ((double) img->width - 2));
	y = round(v * ((double) img->height - 2));
	pos = (y * img->width + x) * 4;
	color = get_clr(img, pos);
	if (blur != 0)
		mix_colors(&color, get_nearby_pixels(img, x, y), blur);
	return (color);
}
