/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_texture.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 15:34:53 by ljylhank          #+#    #+#             */
/*   Updated: 2025/02/19 12:54:35 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <stdlib.h>

static inline void	null_and_trim(t_shape *shape, const char **line)
{
	shape->texture = NULL;
	shape->normal_map = NULL;
	shape->roughness_map = NULL;
	shape->roughness = 0.5;
	*line = trim_left(*line);
}

char	*parse_texture(t_minirt *m, const char *line, t_shape *shape)
{
	int			i;
	char		*filename;
	bool		name_is_empty;

	null_and_trim(shape, &line);
	if (ft_isdigit(*line) || *line == '+' || *line == '-' || *line == '.')
		line = parse_float(m, &shape->roughness, line, -1);
	expect_range(m, vec3(shape->roughness, 0, 1), "Roughness");
	if (*line == '#')
		return ((char *) line);
	filename = ft_arena_alloc(&m->arena, 210);
	i = -1;
	name_is_empty = true;
	while (line[++i] && line[i] != '#' && i < 199)
	{
		if (!ft_isspace(line[i]))
			name_is_empty = false;
		filename[i] = line[i];
	}
	filename[i] = '\0';
	if (name_is_empty)
		return ((char *) line);
	mrt_assert(m, !(i == 199 && line[i]), "texture path too long (200+)");
	shape->texture = (mlx_image_t *)filename;
	return ((char *) line + i);
}
