/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 10:27:22 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/12 21:13:19 by ljylhank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "shapes.h"
# include "ray.h"
# include <.MLX42.h>
# include <libft.h>
# include <stdbool.h>
# include <stdint.h>

# ifndef MRT_FATAL_EXPECT
#  define MRT_FATAL_EXPECT 1
# endif

// Odd INIT_WIDTH recommended for smooth (non-stripey) initial render
# define INIT_WIDTH 1597
# define INIT_HEIGHT 1200

typedef struct s_light
{
	t_vec3	coords;
	t_vec3	color;
}	t_light;

typedef struct s_minirt
{
	mlx_t			*mlx;
	mlx_image_t		*img;
	t_arena			arena;
	char			*line;
	int32_t			mouse_x;
	int32_t			mouse_y;
	bool			cursor_pointing;
	bool			valid_pixel[8];
	size_t			valid_pixel_i;

	t_vec3			ambient_light;
	t_vec3			camera_coords;
	t_vec3			camera_orientation;
	double			camera_field_of_view;
	double			cam_rot_matrix[3][3];
	double			aspect_ratio;

	t_light			*lights;
	size_t			lights_length;

	t_sphere		*spheres;
	size_t			spheres_length;
	t_plane			*planes;
	size_t			planes_length;
	t_cylinder		*cylinders;
	size_t			cylinders_length;
	t_disc			*discs;
	size_t			discs_length;
}	t_minirt;

void		mrt_init(t_minirt *m, const char *path);
void		mrt_destroy(t_minirt *m);
void		mrt_exit(t_minirt *m, int status);
bool		mrt_assert(t_minirt *m, bool condition, const char *msg);
bool		mrt_expect(t_minirt *m, bool condition, const char *msg);

void		parse_input(t_minirt *m, const char *path);
bool		parse_ambient_light(t_minirt *m, const char *line);
bool		parse_camera(t_minirt *m, const char *line);
bool		parse_light(t_minirt *m, const char *line);
void		parse_sphere(t_minirt *m, const char *line);
void		parse_plane(t_minirt *m, const char *line);
void		parse_cylinder(t_minirt *m, const char *line);
char		*parse_float(t_minirt *m, double *f, const char *line, char sep);
char		*parse_texture(t_minirt *m, const char *line, t_shape *shape);
bool		expect_range(t_minirt *m, t_vec3 inputs, const char *name);
t_vec3		expect_normalized(t_minirt *m, t_vec3 v, const char *name);
double		str_to_f(const char *str);
char		*f_to_str(char buf[static 32], double f);
char		*trim_left(const char *str);

void		free_textures(t_minirt *m);
t_vec2		get_plane_uv(t_vec3 intersect, t_plane *plane, int shape);
t_vec2		get_cylinder_uv(t_vec3 intersect, t_cylinder *cylinder);
t_vec2		get_sphere_uv(t_vec3 intersect, t_sphere *sphere);
t_vec3		get_texture_from_uv(mlx_image_t *img, double u, double v,
				double blur);
mlx_image_t	*load_texture(t_minirt *m, char *filename, int texture_type);
t_vec3		get_texture_color(t_vec3 r, int texture_type,
				const t_shape *shape, int shape_type);
double	get_rough_value(t_vec3 r,
			const t_shape *shape, int shape_type);
t_vec3	get_albedo_blur(t_vec3 intersect, const t_shape *shape,
			int shape_type, double blur);

void	key_hook(mlx_key_data_t key, void *minirt);
void	resize_hook(int w, int h, void *minirt);
void	cursor_hook(double x, double y, void *minirt);
void	mouse_hook(mouse_key_t b, action_t a, modifier_key_t m, void *minirt);
void	render_frame(void *rt_voidptr);
void	cast_rays(t_minirt *minirt);

void	get_shape_intersect_dist(t_minirt *m, t_ray *ray, const t_shape *skip);

void	mrt_print_vec3(t_minirt *m, const char *name, t_vec3 v);
void	mrt_print_double(t_minirt *m, const char *name, double x);
#define mrt_print(X) _Generic(X, \
	t_vec3: mrt_print_vec3, \
	double: mrt_print_double)(m, #X, X)

// To debug a specific pixel with GDB, call `mrt_debug()` wherever you want to
// debug. Then, put a breakpoint on `mrt_break()` and click the pixel you want
// to debug.
void	mrt_debug(t_minirt *m);
void	mrt_break(void);

#endif
