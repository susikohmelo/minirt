/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_matrices.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 20:22:06 by ljylhank          #+#    #+#             */
/*   Updated: 2025/02/19 17:38:28 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vec3.h"

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

t_vec3	vec3_mat3(t_vec3 v, const double m[3][3])
{
	return ((t_vec3){
		.x = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0],
		.y = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1],
		.z = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2],
	});
}

t_vec3	mat3_vec3(const double m[3][3], t_vec3 v)
{
	return ((t_vec3){
		.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
		.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
		.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z,
	});
}
