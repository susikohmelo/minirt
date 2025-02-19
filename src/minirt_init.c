/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 11:48:45 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/19 14:27:57 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "get_next_line.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

static void	get_shape_buf_sizes(
	t_minirt *m, size_t sizes[static SHAPES_LENGTH], const char *path)
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

static void	load_textures(t_minirt *m, const char *exec_path)
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
	load_skybox(m, exec_path);
}

static void	init_mlx(t_minirt *m, const char *exec_path)
{
	int32_t	max_w;
	int32_t	max_h;

	m->mlx = mlx_init(INIT_WIDTH, INIT_HEIGHT, "miniRT", true);
	load_textures(m, exec_path);
	mrt_assert(m, m->mlx != NULL, "mlx_init() failed");
	m->img = mlx_new_image(m->mlx, INIT_WIDTH, INIT_HEIGHT);
	mrt_assert(m, m->img != NULL, "mlx_new_image() failed");
	mrt_assert(m, mlx_image_to_window(m->mlx, m->img, 0, 0) != -1, \
		"mlx_image_to_window() failed");
	m->gui_text = mlx_new_image(\
		m->mlx, CHAR_WIDTH * LINE_LENGTH, CHAR_HEIGHT * 32);
	mrt_assert(m, m->gui_text != NULL, "mlx_new_image() failed");
	mrt_assert(m, mlx_image_to_window(m->mlx, m->gui_text, 0, 0) != -1, \
		"mlx_image_to_window() failed");
	mlx_key_hook(m->mlx, key_hook, m);
	mlx_resize_hook(m->mlx, resize_hook, m);
	mlx_cursor_hook(m->mlx, cursor_hook, m);
	mlx_mouse_hook(m->mlx, mouse_hook, m);
	mlx_get_monitor_size(0, &max_w, &max_h);
	mlx_set_window_limit(\
		m->mlx, CHAR_WIDTH * LINE_LENGTH, CHAR_HEIGHT * 11, max_w, max_h);
}

void	mrt_init(t_minirt *m, const char *exec_path, const char *path)
{
	size_t	i;
	size_t	sizes[SHAPES_LENGTH];

	ft_memset(sizes, 0, sizeof sizes);
	get_shape_buf_sizes(m, sizes, path);
	m->lights = ft_arena_calloc(&m->arena, sizes[0], sizeof m->lights[0]);
	m->spheres = ft_arena_calloc(\
		&m->arena, sizes[SHAPE_SPHERE], sizeof m->spheres[0]);
	m->planes = ft_arena_calloc(\
		&m->arena, sizes[SHAPE_PLANE], sizeof m->planes[0]);
	m->cylinders = ft_arena_calloc(\
		&m->arena, sizes[SHAPE_CYLINDER], sizeof m->cylinders[0]);
	m->discs = ft_arena_calloc(\
		&m->arena, 2 * sizes[SHAPE_CYLINDER], sizeof m->discs[0]);
	m->valid_pixel_len = DEFAULT_VALID_PIXEL_LEN;
	parse_input(m, path);
	init_mlx(m, exec_path);
	i = (size_t) - 1;
	while (++i < THREADS)
		m->thrds_data[i] = (t_thread_data){m, i, false};
	i = (size_t) - 1;
	while (++i < THREADS)
		pthread_create(&m->thrds[i], NULL, cast_some_rays, &m->thrds_data[i]);
}
