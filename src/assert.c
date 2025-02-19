/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assert.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:26:26 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/19 14:27:11 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

void	mrt_exit(t_minirt *m, int status)
{
	size_t	i;

	m->should_quit = true;
	i = (size_t) - 1;
	while (++i < THREADS)
		pthread_join(m->thrds[i], NULL);
	if (m->mlx != NULL)
	{
		if (m->img != NULL)
			mlx_delete_image(m->mlx, m->img);
		if (m->gui_text != NULL)
			mlx_delete_image(m->mlx, m->gui_text);
	}
	free_textures(m);
	free(m->mlx);
	free(m->line);
	ft_arena_clear(&m->arena);
	exit(status);
}

bool	mrt_expect(t_minirt *m, bool condition, const char *msg)
{
	int	errno_value;

	errno_value = errno;
	if (condition == true)
		return (true);
	if (MRT_FATAL_EXPECT)
		ft_putstr_fd("Error\n", STDERR_FILENO);
	else
		ft_putstr_fd("Warning\n", STDERR_FILENO);
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
	if (MRT_FATAL_EXPECT)
		mrt_exit(m, EXIT_FAILURE);
	return (false);
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
