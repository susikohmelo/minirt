/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_scalar.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 14:31:25 by lfiestas          #+#    #+#             */
/*   Updated: 2025/01/31 14:31:54 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vec3.h"

t_vec3	vec3_adds(t_vec3 v, double s)
{
	return ((t_vec3){.x = v.x + s, .y = v.y + s, .z = v.z + s, .w = v.w + s});
}

t_vec3	vec3_subs(t_vec3 v, double s)
{
	return ((t_vec3){.x = v.x - s, .y = v.y - s, .z = v.z - s, .w = v.w - s});
}

t_vec3	vec3_muls(t_vec3 v, double s)
{
	return ((t_vec3){.x = v.x * s, .y = v.y * s, .z = v.z * s, .w = v.w * s});
}

t_vec3	vec3_divs(t_vec3 v, double s)
{
	return ((t_vec3){.x = v.x / s, .y = v.y / s, .z = v.z / s, .w = v.w / s});
}
