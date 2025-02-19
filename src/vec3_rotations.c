/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_rotations.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:31:37 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/19 14:38:05 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vec3.h"
#include <math.h>

t_vec3	vec3_rotatex(t_vec3 v, double r)
{
	const double	m[3][3] = \
		{{1, 0, 0}, {0, cos(r), -sin(r)}, {0, sin(r), cos(r)}};

	return (mat3_vec3(m, v));
}

t_vec3	vec3_rotatey(t_vec3 v, double r)
{
	const double	m[3][3] = \
		{{cos(r), 0, sin(r)}, {0, 1, 0}, {-sin(r), 0, cos(r)}};

	return (mat3_vec3(m, v));
}

t_vec3	vec3_rotatez(t_vec3 v, double r)
{
	const double	m[3][3] = \
		{{cos(r), -sin(r), 0}, {sin(r), cos(r), 0}, {0, 0, 1}};

	return (mat3_vec3(m, v));
}

t_vec3	vec3_axis_rotation(t_vec3 v, t_vec3 axis, double r)
{
	const double	m[3][3] = {{
		axis.x * axis.x * (1 - cos(r)) + cos(r),
		axis.x * axis.y * (1 - cos(r)) - axis.z * sin(r),
		axis.x * axis.z * (1 - cos(r)) + axis.y * sin(r)
	}, {
		axis.x * axis.y * (1 - cos(r)) + axis.z * sin(r),
		axis.y * axis.y * (1 - cos(r)) + cos(r),
		axis.y * axis.z * (1 - cos(r)) - axis.x * sin(r)
	}, {
		axis.x * axis.z * (1 - cos(r)) - axis.y * sin(r),
		axis.y * axis.z * (1 - cos(r)) + axis.x * sin(r),
		axis.z * axis.z * (1 - cos(r)) + cos(r)
	}};

	return (mat3_vec3(m, v));
}
