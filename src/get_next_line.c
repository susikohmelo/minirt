/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 08:23:32 by lfiestas          #+#    #+#             */
/*   Updated: 2025/01/06 14:22:29 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

#ifndef OPEN_MAX
# define OPEN_MAX 1024
#endif

char	*get_next_line(int fd)
{
	static t_buffer	bufs[OPEN_MAX];
	t_string		contents;

	if (fd < 0 || fd > OPEN_MAX)
		return (NULL);
	contents = (t_string){};
	while (flush(&contents, bufs[fd].data, bufs[fd].length, &bufs[fd].start))
	{
		bufs[fd].length = read(fd, bufs[fd].data, BUFFER_SIZE);
		if (bufs[fd].length == 0 || bufs[fd].length == (size_t) - 1)
			break ;
	}
	if (bufs[fd].start == bufs[fd].length || bufs[fd].length == (size_t) - 1)
		bufs[fd] = (t_buffer){0};
	return (contents.data);
}
