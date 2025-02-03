/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 11:48:45 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/03 14:27:54 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "get_next_line.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

static void	get_shape_buf_sizes(
	t_minirt *m, size_t sizes[3], const char* path)
{
	int	fd;

	fd = open(path, O_RDONLY);
	mrt_assert(m, fd != -1, path);
	while (true)
	{
		m->line = get_next_line(fd);
		if (m->line == NULL)
			break ;
		while (ft_isspace(*m->line))
			++m->line;
		if (m->line[0] == 's' && m->line[1] == 'p' && ft_isspace(m->line[2]))
			sizes[0]++;
		if (m->line[0] == 'p' && m->line[1] == 'l' && ft_isspace(m->line[2]))
			sizes[1]++;
		if (m->line[0] == 'c' && m->line[1] == 'y' && ft_isspace(m->line[2]))
			sizes[2]++;
		free(m->line);
	}
	close(fd);
}

void	mrt_init(t_minirt *m, const char *path)
{
	size_t	sizes[3];

	get_shape_buf_sizes(m, sizes, path);
	m->spheres = ft_arena_alloc(&m->arena, sizes[0] * sizeof m->spheres[0]);
	m->planes = ft_arena_alloc(&m->arena, sizes[1] * sizeof m->planes[0]);
	m->cylinders = ft_arena_alloc(&m->arena, sizes[2] * sizeof m->cylinders[0]);
	parse_input(m, path);
	m->mlx = mlx_init(INIT_WIDTH, INIT_HEIGHT, "miniRT", true);
	mrt_assert(m, m->mlx != NULL, "mlx_init() failed");
	m->img = mlx_new_image(m->mlx, INIT_WIDTH, INIT_HEIGHT);
	mrt_assert(m, m->img != NULL, "mlx_new_image() failed");
	mrt_assert(m, mlx_image_to_window(m->mlx, m->img, 0, 0) != -1, \
		"mlx_image_to_window() failed");
	mlx_key_hook(m->mlx, key_hook, m);
	mlx_resize_hook(m->mlx, resize_hook, m);
}

void	mrt_destroy(t_minirt *m)
{
	if (m->mlx != NULL && m->img != NULL)
		mlx_delete_image(m->mlx, m->img);
	free(m->mlx);
	free(m->line);
	ft_arena_clear(&m->arena);
}

void	mrt_exit(t_minirt *m, int status)
{
	mrt_destroy(m);
	exit(status);
}

bool	mrt_assert(t_minirt *m, bool condition, const char *msg)
{
	int	errno_value;

	errno_value = errno;
	if (condition == true)
		return (true);
	ft_putstr_fd("Error\n", STDERR_FILENO);
	if (msg != NULL || errno_value != 0)
	{
		if (errno_value != 0)
			ft_putstr_fd(strerror(errno_value), STDERR_FILENO);
		if (errno_value != 0 && msg != NULL)
			ft_putstr_fd(": ", STDERR_FILENO);
		if (msg != NULL)
			ft_putstr_fd(msg, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
	}
	mrt_exit(m, EXIT_FAILURE);
	return (false);
}
