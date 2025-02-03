/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 12:42:23 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/03 13:24:52 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VEC3_H
# define VEC3_H

typedef struct s_vec3
{
	union
	{
		double	x;
		double	r;
	};
	union
	{
		double	y;
		double	g;
	};
	union
	{
		double	z;
		double	b;
	};

	double  _; // TODO check if ymm is actually used in campus machines
}	t_vec3;

t_vec3	vec3(double x, double y, double z);

t_vec3	vec3_add(t_vec3 v1, t_vec3 v2);
t_vec3	vec3_sub(t_vec3 v1, t_vec3 v2);

double	vec3_dot(t_vec3 v1, t_vec3 v2);
t_vec3	vec3_cross(t_vec3 v1, t_vec3 v2);
double	vec3_length(t_vec3 v);
t_vec3	vec3_normalize(t_vec3 v);

t_vec3	vec3_adds(t_vec3 v, double s);
t_vec3	vec3_subs(t_vec3 v, double s);
t_vec3	vec3_muls(t_vec3 v, double s);
t_vec3	vec3_divs(t_vec3 v, double s);

#endif
