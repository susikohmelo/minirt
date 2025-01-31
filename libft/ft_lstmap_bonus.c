/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 09:33:00 by lfiestas          #+#    #+#             */
/*   Updated: 2024/10/31 09:33:16 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

t_list	*ft_lstmap(t_list *in, void *(*f)(void *), void (*del)(void *))
{
	t_list	*out;

	if (in == NULL)
		return (NULL);
	out = malloc(sizeof * out);
	if (out == NULL)
		return (NULL);
	out->content = f(in->content);
	out->next = ft_lstmap(in->next, f, del);
	if (in->next != NULL && out->next == NULL)
	{
		del(out->content);
		free(out);
		out = NULL;
	}
	return (out);
}
