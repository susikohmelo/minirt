/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 11:48:45 by lfiestas          #+#    #+#             */
/*   Updated: 2025/01/31 13:41:31 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void	mrt_init(t_minirt *mrt)
{
	mrt->mlx = mlx_init(INIT_WIDTH, INIT_HEIGHT, "miniRT", true);
	mrt_assert(mrt, mrt->mlx != NULL, "mlx_init() failed");
	mrt->img = mlx_new_image(mrt->mlx, INIT_WIDTH, INIT_HEIGHT);
	mrt_assert(mrt, mrt->img != NULL, "mlx_new_image() failed");
	mrt_assert(mrt, mlx_image_to_window(mrt->mlx, mrt->img, 0, 0) != -1, \
		"mlx_image_to_window() failed");
	mlx_key_hook(mrt->mlx, key_hook, mrt);
	mlx_resize_hook(mrt->mlx, resize_hook, mrt);
}

void	mrt_destroy(t_minirt *mrt)
{
	if (mrt->mlx != NULL && mrt->img != NULL)
		mlx_delete_image(mrt->mlx, mrt->img);
	free(mrt->mlx);
}

void	mrt_exit(t_minirt *mrt, int status)
{
	mrt_destroy(mrt);
	exit(status);
}

bool	mrt_assert(t_minirt *mrt, bool condition, const char *msg)
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
	mrt_exit(mrt, EXIT_FAILURE);
	return (false);
}
