/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arena.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 09:58:57 by lfiestas          #+#    #+#             */
/*   Updated: 2025/01/04 10:10:56 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <unistd.h>

#ifndef PAGE_SIZE
# define PAGE_SIZE 8192
#endif

void	*ft_malloc(size_t size)
{
	void	*p;

	p = malloc(size);
	if (p == NULL)
		exit(EXIT_FAILURE);
	return (p);
}

void	*ft_arena_alloc(t_arena *arena, size_t size)
{
	const size_t	block_size = PAGE_SIZE - sizeof(struct s_arena);
	t_arena			next;
	const size_t	align = sizeof(void *) - 1;
	const size_t	aligned_size = size + align - ((size - 1) & align);

	if (*arena == NULL
		|| (uint8_t *)((*arena)->pos) + aligned_size
		>= (*arena)->mem + block_size)
	{
		next = *arena;
		if (aligned_size < block_size)
			*arena = ft_malloc(PAGE_SIZE);
		else
			*arena = ft_malloc(sizeof(struct s_arena) + aligned_size);
		(*arena)->next = next;
		(*arena)->pos = (*arena)->mem;
	}
	(*arena)->pos += aligned_size;
	return ((*arena)->pos - aligned_size);
}

void	ft_arena_rewind(t_arena *arena, void *pos)
{
	const size_t	block_size = PAGE_SIZE - sizeof(struct s_arena);
	t_arena			next;
	uint8_t			*p;
	const size_t	align = sizeof(void *) - 1;
	const char		msg[] = "ft_arena_rewind(): "
		"pointer not allocated by arena! Crashing...\n\n";

	if (*arena == NULL)
		(int){0} = write(STDERR_FILENO, msg, sizeof msg - sizeof"");
	p = pos;
	if ((*arena)->mem <= p && p <= (*arena)->mem + block_size)
	{
		(*arena)->pos = p + align - (((uintptr_t)p - 1) & align);
		return ;
	}
	next = (*arena)->next;
	free(*arena);
	*arena = next;
	ft_arena_rewind(arena, pos);
}

void	ft_arena_clear(t_arena *arena)
{
	t_arena	next;

	if (*arena == NULL)
		return ;
	next = (*arena)->next;
	free(*arena);
	*arena = NULL;
	ft_arena_clear(&next);
}
