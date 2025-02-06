/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_texture.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 15:34:53 by ljylhank          #+#    #+#             */
/*   Updated: 2025/02/06 16:14:44 by ljylhank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

mlx_image_t	*parse_texture(t_minirt *m, const char *line)
{
	mlx_image_t	*img;
	char		filename[200];
	int			i;

	i = 0;
	while (line[i] && ft_isspace(line[i]) && i < 199)
	{
		filename[i] = line[i];
		++i;
	}
	mrt_assert(m, !(i == 199 && line[i]), "texture path is too long (200+)");
	filename[i] = '\0';
	img = load_texture(m, filename);
	return (img);
}
