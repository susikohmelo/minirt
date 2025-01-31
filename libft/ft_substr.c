/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 09:19:33 by lfiestas          #+#    #+#             */
/*   Updated: 2024/11/01 15:47:35 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_substr(char const *in, unsigned int start, size_t len)
{
	char	*out;
	size_t	i;

	if (start >= ft_strlen(in))
		return (ft_calloc(len, sizeof(char)));
	out = malloc(len + sizeof"");
	if (out == NULL)
		return (NULL);
	i = 0;
	in += start;
	while (in[i] != '\0' && i < len)
	{
		out[i] = in[i];
		i++;
	}
	out[i] = '\0';
	return (out);
}
