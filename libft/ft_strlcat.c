/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 09:12:16 by lfiestas          #+#    #+#             */
/*   Updated: 2024/11/01 12:26:52 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	length;

	if (size == 0)
		return (ft_strlen(src));
	length = ft_strlen(dest);
	i = 0;
	while (src[i] != '\0' && length + i < size - (size > 0))
	{
		dest[length + i] = src[i];
		++i;
	}
	if (length + i < size - 1)
		dest[length + i] = '\0';
	else if (size != 0 && length < size)
		dest[size - 1] = '\0';
	while (src[i] != '\0')
		++i;
	if (length < size)
		return (length + i);
	else
		return (i + size);
}
