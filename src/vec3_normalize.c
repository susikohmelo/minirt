/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_normalize.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 16:50:57 by ljylhank          #+#    #+#             */
/*   Updated: 2025/01/31 16:56:38 by ljylhank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vec3.h"

// TODO will segfault if vector is 0,0,0
t_vec3	vec3_normalize(t_vec3 v)
{
	return (vec3_divs(v, vec3_length(v)));
}
