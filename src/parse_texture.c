/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_texture.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 15:34:53 by ljylhank          #+#    #+#             */
/*   Updated: 2025/02/06 18:58:34 by ljylhank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

mlx_image_t	*parse_texture(t_minirt *m, const char **l)
{
	mlx_image_t	*img;
	char		filename[200];
	int			i;
	const char	*line;
	bool		name_is_empty;

	line = *l;
	i = 0;
	name_is_empty = true;
	while (line[i] && i < 199)
	{
		if (!ft_isspace(line[i]))
			name_is_empty = false;
		filename[i] = line[i];
		++i;
	}
	filename[i] = '\0';
	if (name_is_empty)
		return (NULL);
	mrt_assert(m, !(i == 199 && line[i]), "texture path is too long (200+)");
	img = load_texture(m, filename);
	*l = *l + i;
	return (img);
}
