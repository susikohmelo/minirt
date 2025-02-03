/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:30:17 by lfiestas          #+#    #+#             */
/*   Updated: 2024/12/17 17:01:35 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	arr_push(t_array *arr, const void *elem)
{
	arr_reserve(arr, arr->length + 1);
	ft_memcpy(arr->cs + arr->elem_size * arr->length++, elem, arr->elem_size);
	ft_memset(arr->cs + arr->elem_size * arr->length, 0, arr->elem_size);
}

void	arr_concat(t_array *a, const void *b, size_t b_length)
{
	arr_reserve(a, a->length + b_length);
	ft_memcpy(a->cs + a->elem_size * a->length, b, a->elem_size * b_length);
	a->length += b_length;
	ft_memset(a->cs + a->elem_size * a->length, 0, a->elem_size);
}
