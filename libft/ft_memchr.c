/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 09:14:58 by lfiestas          #+#    #+#             */
/*   Updated: 2024/10/31 09:15:21 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *_s, int c, size_t n)
{
	size_t	i;
	char	*s;

	i = 0;
	s = (char *)_s;
	while (i < n)
	{
		if (s[i] == c)
			return (s + i);
		++i;
	}
	return (NULL);
}
