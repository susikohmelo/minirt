/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shapes.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 13:02:27 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/06 18:58:22 by ljylhank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHAPES_H
# define SHAPES_H

# include "vec3.h"
# include <.MLX42.h>

typedef enum e_shape_type
{
	SHAPE_NO_SHAPE,
	SHAPE_SPHERE,
	SHAPE_PLANE,
	SHAPE_CYLINDER,
}	t_shape_type;

typedef struct s_shape
{
	t_vec3  coords;
	t_vec3	color;
	mlx_image_t *texture;
}	t_shape;

typedef struct s_sphere
{
	t_vec3	coords;
	t_vec3	color;
	mlx_image_t *texture;
	double	radius;
}	t_sphere;

typedef struct s_plane
{
	t_vec3	coords;
	t_vec3	color;
	mlx_image_t *texture;
	t_vec3	normal;
}	t_plane;

typedef struct s_cylinder
{
	t_vec3	coords;
	t_vec3	color;
	mlx_image_t *texture;
	t_vec3	axis;
	double	diameter;
	double	height;
}	t_cylinder;

#endif
