/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_mem.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 18:22:08 by lfiestas          #+#    #+#             */
/*   Updated: 2025/01/07 14:24:37 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

t_array	arr_new(size_t elem_size, size_t init_capacity, t_arena *arena)
{
	t_array	arr;

	ft_memset(&arr, 0, sizeof arr);
	arr.elem_size = elem_size;
	if (arena != NULL)
	{
		arr.cs = ft_arena_alloc(arena, elem_size * init_capacity + elem_size);
		arr.arena = arena;
	}
	else
		arr.cs = ft_malloc(elem_size * init_capacity + elem_size);
	arr.capacity = init_capacity;
	ft_memset(arr.cs, 0, elem_size);
	return (arr);
}

void	arr_delete(t_array *arr)
{
	static const char	null_memory[128] = "";

	if (arr->capacity != 0 && arr->arena == NULL)
		free(arr->cs);
	arr->cs = (char *)null_memory;
	arr->length = 0;
	arr->capacity = 0;
}

/* In case of arr being a string view, we don't want to touch the original
 * memory, thus point to null_memory.
 */
void	arr_reset(t_array *arr)
{
	static const char	null_memory[128] = "";

	if (arr->capacity == 0)
		arr->cs = (char *)null_memory;
	else
		ft_memset(arr->cs, 0, arr->elem_size);
	arr->length = 0;
}

void	arr_reserve(t_array *a, size_t capacity)
{
	void	*tmp;

	if (capacity < a->capacity)
		return ;
	while (a->capacity <= capacity)
		a->capacity = 2 * a->capacity + (a->capacity == 0) * 16;
	if (a->arena != NULL && a->cs + a->capacity == (*a->arena)->pos)
	{
		tmp = ft_arena_alloc(a->arena, a->elem_size * (a->capacity + 1));
		if (tmp != a->cs)
			ft_memcpy(tmp, a->cs, a->elem_size * (a->length + sizeof""));
	}
	else
	{
		if (a->arena == NULL)
			tmp = ft_malloc(a->elem_size * a->capacity + a->elem_size);
		else
			tmp = ft_arena_alloc(a->arena, a->elem_size * (a->capacity + 1));
		ft_memcpy(tmp, a->cs, a->elem_size * a->length);
		if (a->capacity != 0 && a->arena == NULL)
			free(a->cs);
	}
	a->cs = tmp;
	ft_memset(a->cs + a->elem_size * a->length, 0, a->elem_size);
}
