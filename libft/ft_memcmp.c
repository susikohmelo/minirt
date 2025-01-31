/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 09:15:30 by lfiestas          #+#    #+#             */
/*   Updated: 2024/11/01 11:18:29 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *_m1, const void *_m2, size_t size)
{
	const unsigned char	*m1;
	const unsigned char	*m2;
	size_t				i;

	m1 = _m1;
	m2 = _m2;
	i = 0;
	while (i < size)
	{
		if (m1[i] != m2[i])
			return (m1[i] - m2[i]);
		++i;
	}
	return (0);
}
