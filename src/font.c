/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   font.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 17:09:32 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/14 18:56:58 by ljylhank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	draw_char(t_minirt *m, int32_t texoffset, int32_t imgoffset)
{
	uint8_t				*pixelx;
	uint8_t				*pixeli;
	size_t				y;
	size_t				bpp;
	const mlx_texture_t	*font;

	if (texoffset < 0)
		return ;
	font = mlx_get_font();
	bpp = font->bytes_per_pixel;
	y = (size_t) - 1;
	while (++y < font->height)
	{
		pixelx = &font->pixels[(y * font->width + texoffset) * bpp];
		pixeli = m->img->pixels + ((y* m->img->width + imgoffset) * bpp);
		ft_memcpy(pixeli, pixelx, font->width * bpp);
	}
}

void	render_string(t_minirt *m, const char *str, int32_t x, int32_t y)
{
	char			buf[LINE_LENGTH + 1];
	const size_t	len = ft_strlen(str);
	size_t			i;

	x = x + 0; y = y + 0; // TODO TEMP WERROR WORKAROUND
	ft_strlcpy(buf, str, sizeof buf);
	i = (size_t) - 1;
	while (++i < len)
		draw_char(m, mlx_get_texoffset(str[i]), i * CHAR_WIDTH);
}
