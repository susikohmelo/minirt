/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shapes.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 13:02:27 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/12 16:29:20 by ljylhank         ###   ########.fr       */
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
	SHAPE_DISC,
	SHAPES_LENGTH,
}	t_shape_type;

typedef enum e_texture_type
{
	ALBEDO,
	NORMAL_MAP,
	ROUGHNESS_MAP
}	t_texture_type;

typedef struct s_shape
{
	t_vec3		coords;
	t_vec3		color;
	mlx_image_t *texture;
	mlx_image_t *normal_map;
	mlx_image_t *roughness_map;
	double		default_rough;
}	t_shape;

typedef struct s_sphere
{
	t_vec3		coords;
	t_vec3		color;
	mlx_image_t *texture;
	mlx_image_t *normal_map;
	mlx_image_t *roughness_map;
	double		default_rough;
	double		radius;
}	t_sphere;

typedef struct s_plane
{
	t_vec3		coords;
	t_vec3		color;
	mlx_image_t *texture;
	mlx_image_t *normal_map;
	mlx_image_t *roughness_map;
	double		default_rough;
	t_vec3		normal;
}	t_plane;

typedef struct s_cylinder
{
	t_vec3		coords;
	t_vec3		color;
	mlx_image_t *texture;
	mlx_image_t *normal_map;
	mlx_image_t *roughness_map;
	double		default_rough;
	t_vec3		axis;
	double		radius;
	double		height;
}	t_cylinder;

typedef struct s_disc
{
	t_vec3		coords;
	t_vec3		color;
	mlx_image_t *texture;
	mlx_image_t *normal_map;
	mlx_image_t *roughness_map;
	double		default_rough;
	t_vec3		normal;
	double		radius;
}	t_disc;

#endif
