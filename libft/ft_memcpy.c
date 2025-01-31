/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 09:10:35 by lfiestas          #+#    #+#             */
/*   Updated: 2024/10/31 09:10:57 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *_dest, const void *_src, size_t n)
{
	char		*dest;
	const char	*src;

	dest = _dest;
	src = _src;
	while (n > 0)
	{
		dest[n - 1] = src[n - 1];
		--n;
	}
	return (_dest);
}
