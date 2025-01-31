/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 09:16:00 by lfiestas          #+#    #+#             */
/*   Updated: 2024/11/05 14:02:18 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *str, const char *to_find, size_t n)
{
	size_t	i;

	if (str == to_find || (str == NULL && n == 0))
		return ((char *)str);
	if (to_find[0] == '\0')
		return ((char *)str);
	while (*str != '\0' && n != 0)
	{
		i = 0;
		while (str[i] == to_find[i] && to_find[i] != '\0' && n - i != 0)
		{
			++i;
			if (to_find[i] == '\0')
				return ((char *)str);
		}
		++str;
		--n;
	}
	return (NULL);
}
