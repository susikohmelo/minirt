/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fast_strdup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 11:58:21 by lfiestas          #+#    #+#             */
/*   Updated: 2025/01/04 12:00:24 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_fast_strdup(t_arena *arena, const char *s_in)
{
	char	*s_out;
	size_t	length;

	length = ft_strlen(s_in);
	s_out = ft_arena_alloc(arena, length + sizeof"");
	return (ft_memcpy(s_out, s_in, length + sizeof""));
}
