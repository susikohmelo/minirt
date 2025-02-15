/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   font.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 17:09:32 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/14 18:07:38 by lfiestas         ###   ########.fr       */
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
	while (++y < CHAR_HEIGHT)
	{
		pixelx = &font->pixels[(y * font->width + texoffset) * bpp];
		pixeli = m->gui_text->pixels + ((y * m->gui_text->width + imgoffset) * bpp);
		ft_memcpy(pixeli, pixelx, CHAR_WIDTH * bpp);
	}
}

void	render_string(t_minirt *m, const char *str)
{
	char			buf[LINE_LENGTH + 1];
	size_t			i;
	static size_t	line;

	if (str == NULL)
	{
		ft_memset(m->gui_text->pixels, '\0', mlx_get_font()->bytes_per_pixel \
			* m->gui_text->width * m->gui_text->height);
		line = 0;
		return ;
	}
	ft_strlcpy(buf, str, sizeof buf);
	i = (size_t) - 1;
	while (++i < ft_strlen(str))
		draw_char(m, \
			mlx_get_texoffset(str[i]), \
			i * CHAR_WIDTH + line * CHAR_HEIGHT * m->gui_text->width);
	++line;
}
