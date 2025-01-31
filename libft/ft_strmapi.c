/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 09:23:50 by lfiestas          #+#    #+#             */
/*   Updated: 2024/10/31 09:24:13 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strmapi(char const *in, char (*f)(unsigned int, char))
{
	char			*out;
	size_t			length;
	unsigned int	i;

	length = ft_strlen(in);
	out = malloc(length + sizeof"");
	if (out == NULL)
		return (NULL);
	i = 0;
	while (in[i] != '\0')
	{
		out[i] = f(i, in[i]);
		++i;
	}
	out[length] = '\0';
	return (out);
}
