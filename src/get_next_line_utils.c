/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 08:24:03 by lfiestas          #+#    #+#             */
/*   Updated: 2024/12/28 16:42:43 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdlib.h>

static void	str_reserve(t_string *str, size_t capacity)
{
	char	*temp;
	size_t	i;

	if (capacity >= str->capacity)
	{
		while (capacity >= str->capacity)
			str->capacity = 2 * str->capacity + BUFFER_SIZE * !str->capacity;
		temp = malloc(str->capacity + sizeof"");
		i = 0;
		while (temp != NULL && i < str->length)
		{
			temp[i] = str->data[i];
			++i;
		}
		free(str->data);
		str->data = temp;
	}
}

t_string	*str_concat(t_string *str, const char *cstr, size_t cstr_length)
{
	size_t	i;

	str_reserve(str, str->length + cstr_length);
	if (str->data == NULL)
		return (NULL);
	i = 0;
	while (i < cstr_length)
	{
		str->data[str->length++] = cstr[i];
		++i;
	}
	str->data[str->length] = '\0';
	return (str);
}

bool	flush(t_string *out, char *buf, size_t buf_length, size_t *start)
{
	size_t	length;
	size_t	segment_length_including_newline;

	if (buf_length == 0)
		return (true);
	length = 0;
	while (*start + length < buf_length && buf[*start + length] != '\n')
		++length;
	if (*start + length < buf_length)
	{
		segment_length_including_newline = length + 1;
		str_concat(out, buf + *start, segment_length_including_newline);
		*start += segment_length_including_newline;
		return (false);
	}
	str_concat(out, buf + *start, length);
	*start = 0;
	return (true);
}
