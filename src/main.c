/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 10:28:44 by lfiestas          #+#    #+#             */
/*   Updated: 2025/01/31 20:40:02 by ljylhank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	main(int argc, char **argv)
{
	static t_minirt	mrt;

	mrt_assert(&mrt, argc == 2, "You must pass a single argument");
	mrt_assert(&mrt, ft_strlen(argv[1]) >= ft_strlen(".rt") \
		&& ft_strcmp(argv[1] + ft_strlen(argv[1]) - 3, ".rt") == 0, \
		"You must pass a file with `.rt` file extension");
	mrt_init(&mrt);
	mlx_loop_hook(mrt.mlx, render_frame, &mrt);
	mlx_image_to_window(mrt.mlx, mrt.img, 0, 0);
	mlx_loop(mrt.mlx);
	mrt_destroy(&mrt);
}
