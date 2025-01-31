/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 09:18:06 by lfiestas          #+#    #+#             */
/*   Updated: 2024/10/31 09:18:16 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

void	*ft_calloc(size_t count, size_t element_size)
{
	void	*result;

	result = malloc(count * element_size);
	if (result == NULL)
		return (NULL);
	return (ft_memset(result, 0, count * element_size));
}
