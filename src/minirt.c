/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 11:48:45 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/17 11:15:53 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "get_next_line.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

static void	get_shape_buf_sizes(
	t_minirt *m, size_t sizes[static SHAPES_LENGTH], const char* path)
{
	int		fd;
	char	*line;

	fd = open(path, O_RDONLY);
	mrt_assert(m, fd != -1, path);
	while (true)
	{
		m->line = get_next_line(fd);
		if (m->line == NULL)
			break ;
		line = m->line;
		while (ft_isspace(*line))
			++line;
		if (line[0] == 's' && line[1] == 'p' && ft_isspace(line[2]))
			sizes[SHAPE_SPHERE]++;
		else if (line[0] == 'p' && line[1] == 'l' && ft_isspace(line[2]))
			sizes[SHAPE_PLANE]++;
		else if (line[0] == 'c' && line[1] == 'y' && ft_isspace(line[2]))
			sizes[SHAPE_CYLINDER]++;
		else if (line[0] == 'L' && ft_isspace(line[1]))
			sizes[0]++;
		free(m->line);
	}
	close(fd);
}

static void	load_shape_texture(t_minirt *m, t_shape *shape)
{
	char	filename[210];

	if (shape->texture == NULL)
		return ;
	ft_memcpy(filename, shape->texture, sizeof filename);
	shape->texture = load_texture(m, filename, ALBEDO);
	ft_strlcat(filename, ".normal", sizeof filename);
	shape->normal_map = load_texture(m, filename, NORMAL_MAP);
	filename[ft_strlen(filename) - ft_strlen(".normal")] = '\0';
	ft_strlcat(filename, ".rough", sizeof filename);
	shape->roughness_map = load_texture(m, filename, ROUGHNESS_MAP);
}

static void	load_skybox(t_minirt *m)
{
	const char	error_message[] = "loading skybox failed";

	m->skybox.left = load_texture(m, "skybox/skybox.left.xpm42", ALBEDO);
	mrt_assert(m, m->skybox.left, error_message);
	m->skybox.front = load_texture(m, "skybox/skybox.front.xpm42", ALBEDO);
	mrt_assert(m, m->skybox.front, error_message);
	m->skybox.right = load_texture(m, "skybox/skybox.right.xpm42", ALBEDO);
	mrt_assert(m, m->skybox.right, error_message);
	m->skybox.up = load_texture(m, "skybox/skybox.up.xpm42", ALBEDO);
	mrt_assert(m, m->skybox.up, error_message);
	m->skybox.back = load_texture(m, "skybox/skybox.back.xpm42", ALBEDO);
	mrt_assert(m, m->skybox.back, error_message);
	m->skybox.down = load_texture(m, "skybox/skybox.down.xpm42", ALBEDO);
	mrt_assert(m, m->skybox.down, error_message);
	m->max_ray_bounces = DEFAULT_MAX_RAY_BOUNCES;
	m->disable_skybox = SKYBOX_DISABLED_BY_DEFAULT;
}
static void	load_textures(t_minirt *m)
{
	size_t	i;

	i = (size_t) - 1;
	while (++i < m->spheres_length)
		load_shape_texture(m, (t_shape *)&m->spheres[i]);
	i = (size_t) - 1;
	while (++i < m->planes_length)
		load_shape_texture(m, (t_shape *)&m->planes[i]);
	i = (size_t) - 1;
	while (++i < m->cylinders_length)
		load_shape_texture(m, (t_shape *)&m->cylinders[i]);
	i = (size_t) - 1;
	while (++i < m->discs_length)
		load_shape_texture(m, (t_shape *)&m->discs[i]);
	load_skybox(m);
}

void	mrt_init(t_minirt *m, const char *path)
{
	int32_t max_w;
	int32_t	max_h;
	size_t	sizes[SHAPES_LENGTH];

	ft_memset(sizes, 0, sizeof sizes);
	get_shape_buf_sizes(m, sizes, path);
	m->lights = ft_arena_calloc(&m->arena, sizes[0], sizeof m->lights[0]);
	m->spheres = ft_arena_calloc( \
		&m->arena, sizes[SHAPE_SPHERE], sizeof m->spheres[0]);
	m->planes = ft_arena_calloc( \
		&m->arena, sizes[SHAPE_PLANE], sizeof m->planes[0]);
	m->cylinders = ft_arena_calloc( \
		&m->arena, sizes[SHAPE_CYLINDER], sizeof m->cylinders[0]);
	m->discs = ft_arena_calloc( \
		&m->arena, 2 * sizes[SHAPE_CYLINDER], sizeof m->discs[0]);
	parse_input(m, path);
	m->mlx = mlx_init(INIT_WIDTH, INIT_HEIGHT, "miniRT", true);
	load_textures(m);
	mrt_assert(m, m->mlx != NULL, "mlx_init() failed");
	m->img = mlx_new_image(m->mlx, INIT_WIDTH, INIT_HEIGHT);
	mrt_assert(m, m->img != NULL, "mlx_new_image() failed");
	mrt_assert(m, mlx_image_to_window(m->mlx, m->img, 0, 0) != -1, \
		"mlx_image_to_window() failed");
	m->gui_text = mlx_new_image( \
		m->mlx, CHAR_WIDTH * LINE_LENGTH, CHAR_HEIGHT * 32);
	mrt_assert(m, m->gui_text != NULL, "mlx_new_image() failed");
	mrt_assert(m, mlx_image_to_window(m->mlx, m->gui_text, 0, 0) != -1, \
		"mlx_image_to_window() failed");
	mlx_key_hook(m->mlx, key_hook, m);
	mlx_resize_hook(m->mlx, resize_hook, m);
	mlx_cursor_hook(m->mlx, cursor_hook, m);
	mlx_mouse_hook(m->mlx, mouse_hook, m);

	mlx_get_monitor_size(0, &max_w, &max_h);
	mlx_set_window_limit( \
		m->mlx, CHAR_WIDTH * LINE_LENGTH, CHAR_HEIGHT * 11, max_w, max_h);
}

void	mrt_destroy(t_minirt *m)
{
	if (m->mlx != NULL)
	{
		if (m->img != NULL)
			mlx_delete_image(m->mlx, m->img);
		if (m->gui_text != NULL)
			mlx_delete_image(m->mlx, m->gui_text);
	}
	free_textures(m);
	free(m->mlx);
	free(m->line);
	ft_arena_clear(&m->arena);
}

void	mrt_exit(t_minirt *m, int status)
{
	mrt_destroy(m);
	exit(status);
}

bool	mrt_expect(t_minirt *m, bool condition, const char *msg)
{
	int	errno_value;

	errno_value = errno;
	if (condition == true)
		return (true);
	if (MRT_FATAL_EXPECT)
		ft_putstr_fd("Error\n", STDERR_FILENO);
	else
		ft_putstr_fd("Warning\n", STDERR_FILENO);
	if (msg != NULL || errno_value != 0)
	{
		if (errno_value != 0)
			ft_putstr_fd(strerror(errno_value), STDERR_FILENO);
		if (errno_value != 0 && msg != NULL)
			ft_putstr_fd(": ", STDERR_FILENO);
		if (msg != NULL)
			ft_putstr_fd(msg, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
	}
	if (MRT_FATAL_EXPECT)
		mrt_exit(m, EXIT_FAILURE);
	return (false);
}

bool	mrt_assert(t_minirt *m, bool condition, const char *msg)
{
	int	errno_value;

	errno_value = errno;
	if (condition == true)
		return (true);
	ft_putstr_fd("Error\n", STDERR_FILENO);
	if (msg != NULL || errno_value != 0)
	{
		if (errno_value != 0)
			ft_putstr_fd(strerror(errno_value), STDERR_FILENO);
		if (errno_value != 0 && msg != NULL)
			ft_putstr_fd(": ", STDERR_FILENO);
		if (msg != NULL)
			ft_putstr_fd(msg, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
	}
	mrt_exit(m, EXIT_FAILURE);
	return (false);
}
