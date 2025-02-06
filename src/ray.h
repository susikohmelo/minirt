/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 15:14:26 by ljylhank          #+#    #+#             */
/*   Updated: 2025/02/06 18:04:29 by ljylhank         ###   ########.fr       */
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
	const void		*shape;
	t_shape_type	shape_type;
}	t_ray;

# endif
