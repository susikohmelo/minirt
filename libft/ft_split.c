/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 09:21:32 by lfiestas          #+#    #+#             */
/*   Updated: 2024/11/06 17:12:18 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static size_t	count_fields(const char *p, char c)
{
	size_t	fields;

	fields = 0;
	while (*p != '\0')
	{
		++fields;
		while (*p != '\0' && *p != c)
			++p;
		if (*p == '\0')
			break ;
		while (*p != '\0' && *p == c)
			++p;
	}
	return (fields);
}

static void	do_the_split(char **outs, size_t outs_len, const char *in, char c)
{
	size_t	length;
	size_t	i;

	i = 0;
	length = 0;
	while (*in != '\0')
	{
		while (in[length] != '\0' && in[length] != c)
			++length;
		outs[i++] = ft_substr(in, 0, length);
		if (i == outs_len)
			break ;
		in += length;
		length = 0;
		while (*in != '\0' && *in == c)
			++in;
	}
}

char	**ft_split(const char *in, char c)
{
	size_t		fields;
	char		**outs;

	while (*in != '\0' && *in == c)
		++in;
	fields = count_fields(in, c);
	outs = malloc((fields + 1) * sizeof outs[1]);
	if (outs == NULL)
		return (NULL);
	do_the_split(outs, fields, in, c);
	outs[fields] = NULL;
	return (outs);
}
