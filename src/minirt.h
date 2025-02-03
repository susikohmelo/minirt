/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 10:27:22 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/03 14:27:47 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "shapes.h"
# include <.MLX42.h>
# include <libft.h>
# include <stdbool.h>
# include <stdint.h>

# define INIT_WIDTH 512
# define INIT_HEIGHT 512

typedef struct s_minirt
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_arena		arena;
	double		ambient_light_ratio;
	t_vec3		ambient_light_color;
	t_vec3		camera_coords;
	t_vec3		camera_orientation;
	double		camera_field_of_view;
	t_vec3		light_coords;
	double		light_ratio;
	t_sphere	*spheres;
	size_t		spheres_length;
	t_plane		*planes;
	size_t		planes_length;
	t_cylinder	*cylinders;
	size_t		cylinders_length;
	char		*line;
}	t_minirt;

void	mrt_init(t_minirt *m, const char *path);
void	mrt_destroy(t_minirt *m);
void	mrt_exit(t_minirt *m, int status);
bool	mrt_assert(t_minirt *m, bool condition, const char *msg);

void	parse_input(t_minirt *m, const char *path);
bool	parse_ambient_light(t_minirt *m, const char *line);
bool	parse_camera(t_minirt *m, const char *line);
bool	parse_light(t_minirt *m, const char *line);
void	parse_sphere(t_minirt *m, const char *line);
void	parse_plane(t_minirt *m, const char *line);
void	parse_cylinder(t_minirt *m, const char *line);
char	*parse_float(t_minirt *m, double *f, const char *line, char sep);
bool	assert_range(t_minirt *m, t_vec3 inputs, const char *name);
double	str_to_f(const char *str);

void	key_hook(mlx_key_data_t key, void *minirt);
void	resize_hook(int w, int h, void *minirt);

#endif
