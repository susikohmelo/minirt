/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 10:27:22 by lfiestas          #+#    #+#             */
/*   Updated: 2025/01/31 17:24:34 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "vec3.h"
# include <.MLX42.h>
# include <libft.h>
# include <stdbool.h>
# include <stdint.h>

# define INIT_WIDTH 512
# define INIT_HEIGHT 512

typedef struct s_sphere
{
	t_vec3	coords;
	double	diameter;
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

typedef enum e_shape_type
{
	SHAPE_SPHERE,
	SHAPE_PLANE,
	SHAPE_CYLINDER,
}	t_shape_type;

typedef struct s_shape
{
	union
	{
		t_sphere	sphere;
		t_plane		plane;
		t_cylinder	cylinder;
	};
	t_shape_type	type;
}	t_shape;

typedef struct s_minirt
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	double		ambient_light_ratio;
	t_vec3		ambient_light_color;
	t_vec3		camera_coords;
	t_vec3		camera_orientation;
	double		camera_field_of_view;
	t_shape		*shapes;
	size_t		shapes_length;
	char		*line;
}	t_minirt;

void	mrt_init(t_minirt *mrt);
void	mrt_destroy(t_minirt *mrt);
void	mrt_exit(t_minirt *mrt, int status);
bool	mrt_assert(t_minirt *mrt, bool condition, const char *msg);

void	parse_input(t_minirt *mrt, const char *path);

void	key_hook(mlx_key_data_t key, void *minirt);
void	resize_hook(int w, int h, void *minirt);

#endif
