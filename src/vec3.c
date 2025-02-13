/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 14:32:49 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/13 13:06:47 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vec3.h"
#include <math.h>

t_vec3	vec3(double x, double y, double z)
{
	return ((t_vec3){ .x = x, .y = y, .z = z });
}

t_vec3	vec3_add(t_vec3 v1, t_vec3 v2)
{
	return ((t_vec3){
		.x = v1.x + v2.x, .y = v1.y + v2.y, .z = v1.z + v2.z, ._ = v1._ + v2._
	});
}

t_vec3	vec3_sub(t_vec3 v1, t_vec3 v2)
{
	return ((t_vec3){
		.x = v1.x - v2.x, .y = v1.y - v2.y, .z = v1.z - v2.z, ._ = v1._ - v2._
	});
}

t_vec3	vec3_mul(t_vec3 v1, t_vec3 v2)
{
	return ((t_vec3){
		.x = v1.x * v2.x, .y = v1.y * v2.y, .z = v1.z * v2.z, ._ = v1._ * v2._
	});
}

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

// TODO ONE OF THESE IS WRONG!
t_vec3  vec3_mat3(t_vec3 v, const double m[3][3])
{
    return ((t_vec3){
       .x = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0],
       .y = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1],
       .z = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2],
    });
}

// TODO ONE OF THESE IS WRONG!
t_vec3  mat3_vec3(const double m[3][3], t_vec3 v)
{
    return ((t_vec3){
       .x = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0],
       .y = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1],
       .z = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2],
    });
}

t_vec3  vec3_rotatex(t_vec3 v, double r)
{
    const double    m[3][3] = {{1,0,0},{0,cos(r),-sin(r)},{0,sin(r),cos(r)}};

    return (mat3_vec3(m, v));
}

t_vec3  vec3_rotatey(t_vec3 v, double r)
{
    const double    m[3][3] = {{cos(r),0,sin(r)},{0,1,0},{-sin(r),0,cos(r)}};

    return (mat3_vec3(m, v));
}

t_vec3  vec3_rotatez(t_vec3 v, double r)
{
    const double    m[3][3] = {{cos(r),-sin(r),0},{sin(r),cos(r),0},{0,0,1}};

    return (mat3_vec3(m, v));
}
