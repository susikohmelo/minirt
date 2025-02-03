/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 12:58:12 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/03 13:11:10 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARRAY_H
# define ARRAY_H

# include "shapes.h"
# include <libft.h>

typedef struct s_array
{
	union
	{
		char		*cs;
		void		**ps;
		t_sphere	*sphs;
		t_plane		*plns;
		t_cylinder	*cyls;
	};
	size_t			elem_size;
	size_t			length;
	size_t			capacity;
	t_arena			*arena;
}	t_array;

t_array	arr_new(size_t elem_size, size_t init_capacity, t_arena *optional);
void	arr_delete(t_array *arr);
void	arr_reset(t_array *arr);
void	arr_reserve(t_array *arr, size_t capacity);
void	arr_push(t_array *arr, const void *elem);
void	arr_concat(t_array *dest, const void *src, size_t src_length);

#endif
