/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 10:27:22 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/18 11:02:53 by lfiestas         ###   ########.fr       */
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

/*
	Maximum times a ray can reflect off of a surface.
	This includes the initial reflection,meaning 0 disables all reflections.
	Keys 1 and 2 can adjust bounces in real time.
*/

# define DEFAULT_MAX_RAY_BOUNCES 3
/*
	Key 3 can also enable/disable this at any time in the program
*/
# define SKYBOX_DISABLED_BY_DEFAULT 1

/*
	Odd INIT_WIDTH recommended for smooth (non-stripey) initial render
*/
# define INIT_WIDTH 1440
# define INIT_HEIGHT 1024

# define DEFAULT_PIXEL_DIVISION 5

# define MOUSE_SENSITIVITY 0.25
# define SCROLL_SENSITIVITY 0.25
# define MOVE_DISTANCE 0.3

// in pixels
# define CHAR_WIDTH 10
# define CHAR_HEIGHT 20

// in characters
# define FLOAT_WIDTH 5
# define LINE_LENGTH 32

// Max value for some attributes like sphere radius
# define SCALE 8

/*
	Do not reorder these, the order:
	left, front, right, up, back, down
	is accessed via indexing in the get_skybox_color function
*/
typedef union s_skybox
{
	struct
	{
		mlx_image_t	*left;
		mlx_image_t	*front;
		mlx_image_t	*right;
		mlx_image_t	*up;
		mlx_image_t	*back;
		mlx_image_t	*down;
	};
	mlx_image_t	*sky_array[6];
}	t_skybox;

typedef struct s_minirt
{
	mlx_t			*mlx;
	mlx_image_t		*img;
	t_arena			arena;
	char			*line;
	int32_t			mouse_x;
	int32_t			mouse_y;
	double			click_x;
	double			click_y;
	bool			cursor_pointing;
	bool			mouse_r_down;
	bool			show_lights;
	bool			resizing;
	uint8_t			moving_slider;

	t_shape			*moving_shape;
	t_vec3			moving_shape_start;

	int				max_ray_bounces;
	bool			disable_skybox;

	t_shape_type	shape_type;
	t_shape			*shape;
	size_t			valid_pixel_len;
	size_t			valid_pixel_x;
	size_t			valid_pixel_y;
	mlx_image_t		*gui_text;
	size_t			gui_line;


	double			ambient_light_ratio;
	t_vec3			ambient_light_color;

	t_skybox		skybox;

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
t_vec3		get_skybox_color(t_minirt *m, t_vec3 dir, double blur);

void	redraw(t_minirt *m, bool flush_black);
void	key_hook(mlx_key_data_t key, void *minirt);
void	resize_hook(int w, int h, void *minirt);
void	cursor_hook(double x, double y, void *minirt);
void	scroll_hook(double x_delta, double y_delta, void *minirt);
void	mouse_hook(mouse_key_t b, action_t a, modifier_key_t m, void *minirt);
void	render_frame(void *rt_voidptr);
void	render_string(t_minirt *m, const char *str);
void	edit_objects(t_minirt *m, double x);
void	move_shape(t_minirt *m, double x, double y);
t_vec3	perpendiculary(t_vec3 v);
t_vec3	perpendicularx(t_vec3 v);

void	cast_rays(t_minirt *minirt);
t_ray	cast_ray(t_minirt *m, size_t column, size_t row);
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
