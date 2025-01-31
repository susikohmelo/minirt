/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 09:13:56 by lfiestas          #+#    #+#             */
/*   Updated: 2024/10/31 09:14:10 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s_start, int c)
{
	const char	*s;

	s = s_start + ft_strlen(s_start);
	if (c == '\0')
		return ((char *)s);
	while (s > s_start)
		if (*--s == c)
			return ((char *)s);
	return (NULL);
}
