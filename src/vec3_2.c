/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 21:31:21 by ljylhank          #+#    #+#             */
/*   Updated: 2025/02/12 21:31:55 by ljylhank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vec3.h"
#include <math.h>

t_vec3	vec3_cross(t_vec3 v1, t_vec3 v2)
{
	return ((t_vec3){
		.x = v1.y * v2.z - v1.z * v2.y,
		.y = v1.z * v2.x - v1.x * v2.z,
		.z = v1.x * v2.y - v1.y * v2.x
	});
}

double	vec3_length(t_vec3 v)
{
	return (sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}

t_vec3	vec3_normalize(t_vec3 v)
{
	return (vec3_divs(v, vec3_length(v)));
}

t_vec3	vec3_inverse_lookat(t_vec3 v1, t_vec3 v2)
{
	t_vec3	f;
	t_vec3	r;
	t_vec3	u;
	t_vec3	t;

	f = v2;
	if (f.y == 1 || f.y == -1)
		r = vec3(1, 0, 0);
	else
		r = vec3_normalize(vec3_cross(vec3(0, 1, 0), f));
	u = vec3_cross(f, r);
	t = v1;
	v1.x = t.x * r.x + t.y * u.x + t.z * f.x;
	v1.y = t.x * r.y + t.y * u.y + t.z * f.y;
	v1.z = t.x * r.z + t.y * u.z + t.z * f.z;
	return (v1);
}

t_vec3	vec3_lookat(t_vec3 v1, t_vec3 v2)
{
	t_vec3	f;
	t_vec3	r;
	t_vec3	u;
	t_vec3	t;

	f = v2;
	if (f.y == 1 || f.y == -1)
		r = vec3(1, 0, 0);
	else
		r = vec3_normalize(vec3_cross(vec3(0, 1, 0), f));
	u = vec3_cross(f, r);
	t = v1;
	v1.x = t.x * r.x + t.y * r.y + t.z * r.z;
	v1.y = t.x * u.x + t.y * u.y + t.z * u.z;
	v1.z = t.x * f.x + t.y * f.y + t.z * f.z;
	return (v1);
}
