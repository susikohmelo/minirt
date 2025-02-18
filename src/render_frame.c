/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_frame.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 21:06:44 by ljylhank          #+#    #+#             */
/*   Updated: 2025/02/18 17:50:42 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <stdio.h>
#include <unistd.h>

static double	moving_average(double x)
{
	static double	xs[16];
	static size_t	i;
	static double	sum;
	const size_t	length = sizeof xs / sizeof xs[0];

	sum += x - xs[i];
	xs[i++] = x;
	if (i >= length)
		i = 0;
	return (sum / length);
}

void	*cast_some_rays(void *thread_data)
{
	t_thread_data	*td;

	td = thread_data;
	while (!td->minirt->should_quit)
	{
		if (!td->done)
		{
			cast_rays(td->minirt, td->id);
			td->done = true;
		}
		usleep(10);
	}
	return (NULL);
}

void	render_frame(void *minirt)
{
	static bool		trues[2048];
	t_minirt		*m;
	static double	t1;
	double			t;
	size_t			i;

	if (trues[0] == false)
		ft_memset(trues, true, sizeof trues);
	m = minirt;
	i = (size_t) - 1;
	while (++i < THREADS)
		m->thrds_data[i].done = false;
	i = (size_t) - 1;
	while (++i < THREADS)
		while (!m->thrds_data[i].done)
			usleep(10);
	m->valid_pixel[m->valid_pixel_i] = true;
	m->valid_pixel_i = (m->valid_pixel_i + 5) & (sizeof m->valid_pixel - 1);
	if (ft_memcmp(m->valid_pixel, trues, sizeof m->valid_pixel) == 0)
		m->resizing = false;
	render_text(m);
	t = mlx_get_time();
	printf("                                                     \r" \
		"FPS: %g\e[1A\n", moving_average(1 / (t - t1)));
	t1 = t;
}
