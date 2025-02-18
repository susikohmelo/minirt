/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 12:42:23 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/18 11:24:53 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VEC3_H
# define VEC3_H

typedef struct s_vec2
{
	double	x;
	double	y;
}	t_vec2;

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
	union
	{
		double	w;
		double	a;
	};
}	t_vec3;

t_vec3	vec3(double x, double y, double z)__attribute__((warn_unused_result));
t_vec3	vec4(double r, double g, double b, double a) \
	__attribute__((warn_unused_result));

t_vec3	vec3_add(t_vec3 v1, t_vec3 v2)__attribute__((warn_unused_result));
t_vec3	vec3_sub(t_vec3 v1, t_vec3 v2)__attribute__((warn_unused_result));
t_vec3	vec3_mul(t_vec3 v1, t_vec3 v2)__attribute__((warn_unused_result));

double	vec3_dot(t_vec3 v1, t_vec3 v2)__attribute__((warn_unused_result));
t_vec3	vec3_cross(t_vec3 v1, t_vec3 v2)__attribute__((warn_unused_result));
double	vec3_length(t_vec3 v)__attribute__((warn_unused_result));
t_vec3	vec3_normalize(t_vec3 v)__attribute__((warn_unused_result));
t_vec3	vec3_clamp(t_vec3 v, double min, double max) \
	__attribute__((warn_unused_result));

t_vec3	vec3_rotate_to_camera(t_vec3 v)__attribute__((warn_unused_result));
t_vec3	vec3_inverse_lookat(t_vec3 v1, t_vec3 v2)__attribute__((warn_unused_result));
t_vec3	vec3_lookat(t_vec3 v1, t_vec3 v2)__attribute__((warn_unused_result));

t_vec3	vec3_adds(t_vec3 v, double s)__attribute__((warn_unused_result));
t_vec3	vec3_subs(t_vec3 v, double s)__attribute__((warn_unused_result));
t_vec3	vec3_muls(t_vec3 v, double s)__attribute__((warn_unused_result));
t_vec3	vec3_divs(t_vec3 v, double s)__attribute__((warn_unused_result));

t_vec3  vec3_mat3( \
    t_vec3 v, const double m[3][3])__attribute__((warn_unused_result));
t_vec3  mat3_vec3( \
    const double m[3][3], t_vec3 v)__attribute__((warn_unused_result));
t_vec3  vec3_rotatex(t_vec3 v, double r)__attribute__((warn_unused_result));
t_vec3  vec3_rotatey(t_vec3 v, double r)__attribute__((warn_unused_result));
t_vec3  vec3_rotatez(t_vec3 v, double r)__attribute__((warn_unused_result));

#endif
