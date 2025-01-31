/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_i_to_str.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 13:41:49 by lfiestas          #+#    #+#             */
/*   Updated: 2025/01/31 11:18:48 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_i_to_str(char result[static 12], int num)
{
	long long		n;
	size_t			length;

	ft_memset(result, '\0', 12);
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
