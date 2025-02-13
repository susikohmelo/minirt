/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 10:28:44 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/12 15:42:52 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// TODO FIX THIS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Also the window should be created AFTER parsing, not before!
/*
➜  minirt_dev git:(more_shapes) ✗ ./miniRT scenes/minimalist.rt
Error
Resource temporarily unavailable: Invalid numerical value

=================================================================
==296493==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 288216 byte(s) in 1 object(s) allocated from:
    #0 0x49a582 in calloc (/home/lfiestas/minirt_dev/miniRT+0x49a582)
    #1 0x4f7171 in mlx_init (/home/lfiestas/minirt_dev/miniRT+0x4f7171)

SUMMARY: AddressSanitizer: 288216 byte(s) leaked in 1 allocation(s).
*/

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
	mlx_loop(m.mlx);
	mrt_destroy(&m);
}
