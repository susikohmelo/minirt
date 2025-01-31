/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 09:20:30 by lfiestas          #+#    #+#             */
/*   Updated: 2024/10/31 09:20:41 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	length1;
	size_t	length2;
	char	*out;

	length1 = ft_strlen(s1);
	length2 = ft_strlen(s2);
	out = malloc(length1 + length2 + sizeof"");
	if (out == NULL)
		return (NULL);
	ft_memcpy(out, s1, length1);
	ft_memcpy(out + length1, s2, length2);
	out[length1 + length2] = '\0';
	return (out);
}
