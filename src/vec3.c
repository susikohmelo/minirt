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
	return ((t_vec3){.x = x, .y = y, .z = z});
}

t_vec3	vec4(double r, double g, double b, double a)
{
	return ((t_vec3){.r = r, .g = g, .b = b, .a = a});
}

t_vec3	vec3_add(t_vec3 v1, t_vec3 v2)
{
	return ((t_vec3){
		.x = v1.x + v2.x, .y = v1.y + v2.y, .z = v1.z + v2.z, .w = v1.w + v2.w
	});
}

t_vec3	vec3_sub(t_vec3 v1, t_vec3 v2)
{
	return ((t_vec3){
		.x = v1.x - v2.x, .y = v1.y - v2.y, .z = v1.z - v2.z, .w = v1.w - v2.w
	});
}

t_vec3	vec3_mul(t_vec3 v1, t_vec3 v2)
{
	return ((t_vec3){
		.x = v1.x * v2.x, .y = v1.y * v2.y, .z = v1.z * v2.z, .w = v1.w * v2.w
	});
}

double	vec3_dot(t_vec3 v1, t_vec3 v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
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
