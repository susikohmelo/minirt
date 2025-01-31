/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 09:17:20 by lfiestas          #+#    #+#             */
/*   Updated: 2024/11/01 13:52:05 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *s)
{
	long long	result;
	int			sign;

	while (*s != '\0' && ft_strchr(" \t\n\v\f\r", *s) != NULL)
		++s;
	sign = 1;
	if (*s == '-')
	{
		sign = -sign;
		++s;
	}
	else if (*s == '+')
		++s;
	result = 0;
	while (ft_isdigit(*s))
		result = result * 10 + *s++ - '0';
	return (sign * result);
}
