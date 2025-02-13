/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 15:14:26 by ljylhank          #+#    #+#             */
/*   Updated: 2025/02/13 12:16:51 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAY_H
# define RAY_H

# include "shapes.h"
# include "vec3.h"

# define TWO_PI 6.28318530718

typedef struct s_ray
{
	t_vec3			start;
	t_vec3			dir;
	double			length;
	const t_shape	*shape;
	t_shape_type	shape_type;
	double			is_reflect;
	bool			inside_shape;
}	t_ray;

# endif
