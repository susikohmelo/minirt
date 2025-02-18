/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_dot_cross_clamp_length_normalize.c            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 21:31:21 by ljylhank          #+#    #+#             */
/*   Updated: 2025/02/18 20:24:56 by ljylhank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vec3.h"
#include <math.h>

double	vec3_dot(t_vec3 v1, t_vec3 v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

t_vec3	vec3_cross(t_vec3 v1, t_vec3 v2)
{
	return ((t_vec3){
		.x = v1.y * v2.z - v1.z * v2.y,
		.y = v1.z * v2.x - v1.x * v2.z,
		.z = v1.x * v2.y - v1.y * v2.x
	});
}

t_vec3	vec3_clamp(t_vec3 v, double min, double max)
{
	return (vec4(\
		fmin(fmax(v.r, min), max), \
		fmin(fmax(v.g, min), max), \
		fmin(fmax(v.b, min), max), \
		fmin(fmax(v.a, min), max)));
}

double	vec3_length(t_vec3 v)
{
	return (sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}

t_vec3	vec3_normalize(t_vec3 v)
{
	return (vec3_divs(v, vec3_length(v)));
}
