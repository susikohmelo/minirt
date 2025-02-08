/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 16:03:43 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/08 12:57:29 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "get_next_line.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

char	*parse_float(t_minirt *m, double *f, const char *line, char separator)
{
	bool	digit_found;

	digit_found = false;
	*f = str_to_f(line);
	if (*line == '+' || *line == '-')
		++line;
	while (ft_isdigit(*line))
		digit_found = ++line;
	if (*line == '.')
		++line;
	while (ft_isdigit(*line))
		digit_found = ++line;
	mrt_assert(m, digit_found, "Number has no digits");
	if (separator == ' ' && mrt_assert(m, ft_isspace(*line), "Invalid number"))
		line = trim_left(line);
	else if (separator == -1)
		mrt_assert(m, *line == '\0' || *line == ' ', "Invalid numerical value");
	else
		mrt_assert(m, *line == separator, "Invalid numerical value");
	return ((char *)line + (separator != ' '));
}

static void	parse_line(t_minirt *m, bool found[128], const char *line)
{
	line = trim_left(line);
	if (line[0] == 'A' && ft_isspace(line[1])
		&& mrt_assert(m, !found['A'], "Multiple ambient lights"))
		found['A'] = parse_ambient_light(m, line + ft_strlen("A "));
	else if (line[0] == 'C' && ft_isspace(line[1])
		&& mrt_assert(m, !found['C'], "Multiple cameras"))
		found['C'] = parse_camera(m, line + ft_strlen("C "));
	else if (line[0] == 'L' && ft_isspace(line[1]))
		found['L'] = parse_light(m, line + ft_strlen("L "));
	else if (line[0] == 's' && line[1] == 'p' && ft_isspace(line[2]))
		parse_sphere(m, line + ft_strlen("sp "));
	else if (line[0] == 'p' && line[1] == 'l' && ft_isspace(line[2]))
		parse_plane(m, line + ft_strlen("pl "));
	else if (line[0] == 'c' && line[1] == 'y' && ft_isspace(line[2]))
		parse_cylinder(m, line + ft_strlen("cy "));
	else
		mrt_assert(m, line[0] == '#', "Unknown element type");
}

void	parse_input(t_minirt *m, const char *path)
{
	int		fd;
	bool	found[128];
	size_t	line_length;

	ft_memset(found, false, sizeof found);
	fd = open(path, O_RDONLY);
	mrt_assert(m, fd != -1, path);
	while (true)
	{
		m->line = get_next_line(fd);
		if (m->line == NULL)
			break ;
		line_length = ft_strlen(m->line);
		while (line_length != 0 && ft_isspace(m->line[line_length - 1]))
			m->line[--line_length] = '\0';
		if (m->line[0] != '\0')
			parse_line(m, found, m->line);
		free(m->line);
	}
	close(fd);
	mrt_assert(m, found['A'], "No ambient light specified");
	mrt_assert(m, found['C'], "No camera specified");
	mrt_assert(m, found['L'], "No light specified");
}
