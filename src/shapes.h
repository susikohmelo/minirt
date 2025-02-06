/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shapes.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 13:02:27 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/06 16:10:22 by ljylhank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHAPES_H
# define SHAPES_H

# include "vec3.h"
# include <.MLX42.h>

typedef struct s_sphere
{
	mlx_image_t	*texture;
	t_vec3		coords;
	double		diameter;
	t_vec3		color;
}	t_sphere;

typedef struct s_plane
{
	mlx_image_t	*texture;
	t_vec3		coords;
	t_vec3		normal;
	t_vec3		color;
}	t_plane;

typedef struct s_cylinder
{
	mlx_image_t	*texture;
	t_vec3		coords;
	t_vec3		axis;
	double		diameter;
	double		height;
	t_vec3		color;
}	t_cylinder;

#endif
