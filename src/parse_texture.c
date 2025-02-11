/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_texture.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 15:34:53 by ljylhank          #+#    #+#             */
/*   Updated: 2025/02/11 15:20:56 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static inline char	*concat_normal(char *filename, int pos)
{
	filename[pos + 0] = '.';
	filename[pos + 1] = 'n';
	filename[pos + 2] = 'o';
	filename[pos + 3] = 'r';
	filename[pos + 4] = 'm';
	filename[pos + 5] = 'a';
	filename[pos + 6] = 'l';
	filename[pos + 7] = '\0';
	return (filename);
}

static inline char	*concat_roughness(char *filename, int pos)
{
	filename[pos + 0] = '.';
	filename[pos + 1] = 'r';
	filename[pos + 2] = 'o';
	filename[pos + 3] = 'u';
	filename[pos + 4] = 'g';
	filename[pos + 5] = 'h';
	filename[pos + 6] = '\0';
	return (filename);
}

char	*parse_texture(t_minirt *m, const char *line, t_shape *shape)
{
	int			i;
	char		filename[210];
	bool		name_is_empty;

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
	mrt_assert(m, !(i == 199 && line[i]), "texture path is too long (200+)");
	shape->texture = load_texture(m, filename, ALBEDO);
	shape->normal_map = load_texture(m, concat_normal(filename, i), NORMAL_MAP);
	shape->roughness_map = load_texture(m, concat_roughness(filename, i), ROUGHNESS_MAP);
	return ((char *) line + i);
}
