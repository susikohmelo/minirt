/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 09:19:01 by lfiestas          #+#    #+#             */
/*   Updated: 2024/10/31 09:19:17 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strdup(const char *s_in)
{
	char	*s_out;
	size_t	length;

	length = ft_strlen(s_in);
	s_out = malloc(length + sizeof"");
	if (s_out == NULL)
		return (NULL);
	return (ft_memcpy(s_out, s_in, length + sizeof""));
}
