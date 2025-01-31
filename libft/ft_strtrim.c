/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 09:21:07 by lfiestas          #+#    #+#             */
/*   Updated: 2024/10/31 09:21:16 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *in, char const *char_set)
{
	size_t	length;
	char	*out;

	while (*in != '\0' && ft_strchr(char_set, *in) != NULL)
		++in;
	length = ft_strlen(in);
	out = ft_strdup(in);
	if (*in == '\0' || out == NULL)
		return (out);
	while (ft_strrchr(char_set, out[--length]))
		out[length] = '\0';
	return (out);
}
