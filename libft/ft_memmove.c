/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 09:11:08 by lfiestas          #+#    #+#             */
/*   Updated: 2024/10/31 09:11:23 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *_dest, void *_src, size_t n)
{
	char		*dest;
	const char	*src;
	size_t		i;

	i = 0;
	dest = _dest;
	src = _src;
	if (src < dest)
	{
		while (n > 0)
		{
			dest[n - 1] = src[n - 1];
			--n;
		}
	}
	else
	{
		while (i < n)
		{
			dest[i] = src[i];
			++i;
		}
	}
	return (_dest);
}
