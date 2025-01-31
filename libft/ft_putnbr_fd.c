/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 09:27:33 by lfiestas          #+#    #+#             */
/*   Updated: 2024/11/27 19:59:37 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>

void	ft_putnbr_fd(int n, int fd)
{
	long long int	num;
	char			digits[16];
	unsigned int	digits_length;

	num = n;
	if (num == 0)
	{
		(ssize_t){0} = write(fd, "0", 1);
		return ;
	}
	if (num < 0)
	{
		(ssize_t){0} = write(fd, "-", 1);
		num *= -1;
	}
	digits_length = 0;
	while (num != 0)
	{
		digits[digits_length++] = num % 10 + '0';
		num /= 10;
	}
	while (digits_length != 0)
		(ssize_t){0} = write(fd, &digits[--digits_length], 1);
}
