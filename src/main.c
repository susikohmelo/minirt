/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 10:28:44 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/05 21:07:06 by ljylhank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	main(int argc, char **argv)
{
	static t_minirt	m;

	mrt_assert(&m, argc == 2, "You must pass a single argument");
	mrt_assert(&m, ft_strlen(argv[1]) >= ft_strlen(".rt") \
		&& ft_strcmp(argv[1] + ft_strlen(argv[1]) - 3, ".rt") == 0, \
		"You must pass a file with `.rt` file extension");
	mrt_init(&m, argv[1]);
	mlx_loop_hook(m.mlx, render_frame, &m);
	xpm_t		*xpm = mlx_load_xpm42("./textures/kiddy.xpm42");
	m.temp = mlx_texture_to_image(m.mlx, &xpm->texture);
	mlx_image_to_window(m.mlx, m.img, 0, 0);
	mlx_loop(m.mlx);
	mrt_destroy(&m);
}
