/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 08:23:35 by lfiestas          #+#    #+#             */
/*   Updated: 2025/01/31 16:09:55 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stddef.h>
# include <stdbool.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 4096
# elif BUFFER_SIZE <= 0 || BUFFER_SIZE >= 1 << 21
#  undef BUFFER_SIZE
#  define BUFFER_SIZE 4096
# endif

typedef struct s_buffer
{
	size_t	length;
	size_t	start;
	char	data[BUFFER_SIZE];
}			t_buffer;

typedef struct s_string
{
	char	*data;
	size_t	length;
	size_t	capacity;
}			t_string;

char		*get_next_line(int fd);

t_string	*str_concat(t_string *str, const char *cstr, size_t cstr_length);
bool		flush(t_string *out, char *buf, size_t buf_length, size_t *start);

#endif
