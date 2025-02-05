/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shapes.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 13:02:27 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/05 13:11:25 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHAPES_H
# define SHAPES_H

# include "vec3.h"

typedef struct s_sphere
{
	t_vec3	coords;
	double	radius;
	t_vec3	color;
}	t_sphere;

typedef struct s_plane
{
	t_vec3	coords;
	t_vec3	normal;
	t_vec3	color;
}	t_plane;

typedef struct s_cylinder
{
	t_vec3	coords;
	t_vec3	axis;
	double	diameter;
	double	height;
	t_vec3	color;
}	t_cylinder;

#endif
