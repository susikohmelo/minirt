/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 09:22:27 by lfiestas          #+#    #+#             */
/*   Updated: 2024/10/31 09:22:51 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_itoa(int num)
{
	long long		n;
	char			*result;
	size_t			length;

	result = ft_calloc(12, sizeof result[0]);
	if (num == 0 && result != NULL)
		result[0] = '0';
	if (result == NULL || (num == 0))
		return (result);
	n = num;
	length = 0;
	if (n < 0)
	{
		result[length++] = '-';
		n = -n;
	}
	while (n != 0 && 1 + length++)
		n /= 10;
	n = (long long)num * (1 - 2 * (num < 0));
	while (n != 0)
	{
		result[--length] = n % 10 + '0';
		n /= 10;
	}
	return (result);
}
