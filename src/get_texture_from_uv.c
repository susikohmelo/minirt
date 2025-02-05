/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_texture_from_uv.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 16:58:10 by ljylhank          #+#    #+#             */
/*   Updated: 2025/02/05 21:10:48 by ljylhank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>
#include <stdio.h>

int set_rgba(int r, int g, int b, int a)
{
    return (r << 24 | g << 16 | b << 8 | a);
}

unsigned int get_texture_from_uv(mlx_image_t *img, double u, double v)
{
	int			pixel;
	int			posx;
	int			posy;
	int			pos;

	posx = img->width - round(u * (double) img->width);
	posy = round(v * (double) img->height);
	pos = (posy * img->width + posx) * 4;
	return (set_rgba(img->pixels[pos], img->pixels[pos + 1], img->pixels[pos + 2], img->pixels[pos + 3]));
}
