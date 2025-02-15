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

static void	blend(uint8_t pixel_a[static 4], const uint8_t pixel_b[static 4])
{
	t_vec3	a;
	t_vec3	b;
	t_vec3	color;
	double	ao;

	b = vec3_divs(vec4(pixel_a[0], pixel_a[1], pixel_a[2], pixel_a[3]), 255);
	a = vec3_divs(vec4(pixel_b[0], pixel_b[1], pixel_b[2], pixel_b[3]), 255);
	ao = a.a + b.a * (1 - a.a);
	color = vec3_divs( \
		vec3_add( \
			vec3_muls(a, a.a), \
			vec3_muls(b, b.a * (1 - a.a))), \
		ao);
	color.a = ao;
	color = vec3_muls(color, 255);
	pixel_a[0] = color.r;
	pixel_a[1] = color.g;
	pixel_a[2] = color.b;
	pixel_a[3] = color.a;
}

static void	draw_char(t_minirt *m, int32_t texoffset, int32_t imgoffset)
{
	uint8_t				*pixelx;
	uint8_t				*pixeli;
	size_t				x;
	size_t				y;
	const mlx_texture_t	*font;

	if (texoffset < 0)
		return ;
	font = mlx_get_font();
	y = (size_t) - 1;
	while (++y < CHAR_HEIGHT)
	{
		pixelx = &font->pixels[(y * font->width + texoffset) * 4];
		pixeli = m->gui_text->pixels + ((y * m->gui_text->width + imgoffset) * 4);
		x = (size_t) - 1;
		while (++x < CHAR_WIDTH)
			blend(pixeli + x * 4, pixelx + x * 4);
	}
}

void	render_string(t_minirt *m, const char *str)
{
	char			buf[LINE_LENGTH + 1];
	size_t			i;

	if (str == NULL)
	{
		ft_memset(m->gui_text->pixels, '\0', mlx_get_font()->bytes_per_pixel \
			* m->gui_text->width * m->gui_text->height);
		m->gui_line = 0;
		return ;
	}
	ft_strlcpy(buf, str, sizeof buf);
	i = (size_t) - 1;
	while (++i < ft_strlen(str))
		draw_char(m, \
			mlx_get_texoffset(str[i]), \
			i * CHAR_WIDTH + m->gui_line * CHAR_HEIGHT * m->gui_text->width);
	++m->gui_line;
}
