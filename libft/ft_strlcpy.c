/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 09:11:35 by lfiestas          #+#    #+#             */
/*   Updated: 2024/10/31 09:11:55 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	while (src[i] != '\0' && i < size - (size > 0))
	{
		dest[i] = src[i];
		++i;
	}
	if (size != 0)
		dest[i] = '\0';
	while (src[i] != '\0')
		++i;
	return (i);
}
