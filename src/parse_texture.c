/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_texture.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 15:34:53 by ljylhank          #+#    #+#             */
/*   Updated: 2025/02/12 12:43:12 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <stdlib.h>

char	*parse_texture(t_minirt *m, const char *line, t_shape *shape)
{
	int			i;
	char		*filename;
	bool		name_is_empty;

	filename = ft_arena_alloc(&m->arena, 210);
	line = trim_left(line);
	shape->texture = NULL;
	shape->normal_map = NULL;
	shape->roughness_map = NULL;
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
		return ((char *) line);
	mrt_assert(m, !(i == 199 && line[i]), "texture path too long (200+)");
	shape->texture = (mlx_image_t *)filename;
	return ((char *) line + i);
}
