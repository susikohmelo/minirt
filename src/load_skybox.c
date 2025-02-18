/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_skybox.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljylhank <ljylhank@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 19:34:58 by ljylhank          #+#    #+#             */
/*   Updated: 2025/02/18 23:27:17 by ljylhank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <unistd.h>

static inline void	append_paths(char *path1, char *path2, char *new_path)
{
	size_t	i;
	size_t	i2;

	i = (size_t) - 1;
	while (path1[++i])
		new_path[i] = path1[i];
	i2 = (size_t) - 1;
	while (path2[++i2])
		new_path[i + i2] = path2[i2];
	new_path[i + i2] = '\0';
}

static inline void	append_newline(char *path, size_t *counter)
{
	if (path[*counter] == '/')
		return ;
	path[*counter] = '/';
	path[++(*counter)] = '\0';
}

static inline void	append_exec_to_cwd(char *buf, const char *exec_path,
						size_t buf_len, size_t exec_len)
{
	int		i;
	int		i2;
	char	last_char;

	append_newline(buf, &buf_len);
	i = -1;
	i2 = 0;
	last_char = 0;
	while (++i < (int) exec_len)
	{
		if (exec_path[i] == '.' && exec_path[i + 1] == '/' && last_char != '.')
			i += 2;
		if (i >= (int) exec_len)
			break ;
		buf[buf_len + i2++] = exec_path[i];
		last_char = exec_path[i];
	}
	buf[buf_len + i2] = '\0';
	while (--i2 >= 0 && buf[buf_len + i2] != '/')
		buf[buf_len + i2] = '\0';
}

static inline void	get_exec_path(t_minirt *m, char *buf, const char *exec_path)
{
	size_t	buf_len;
	size_t	exec_len;

	mrt_assert(m, getcwd(buf, 510), "error getting current path for skybox");
	buf_len = ft_strlen(buf);
	exec_len = ft_strlen(exec_path);
	mrt_assert(m, buf_len != 510 && exec_len != 511, "paths are too long");
	append_exec_to_cwd(buf, exec_path, buf_len, exec_len);
}

void	load_skybox(t_minirt *m, const char *exec_path)
{
	char		cwd[1024];
	char		path[1024];

	get_exec_path(m, cwd, exec_path);
	append_paths(cwd, "skybox/skybox.left.xpm42", path);
	m->skybox.left = load_texture(m, path, ALBEDO);
	mrt_assert(m, m->skybox.left, "loading skybox failed");
	append_paths(cwd, "skybox/skybox.front.xpm42", path);
	m->skybox.front = load_texture(m, path, ALBEDO);
	mrt_assert(m, m->skybox.front, "loading skybox failed");
	append_paths(cwd, "skybox/skybox.right.xpm42", path);
	m->skybox.right = load_texture(m, path, ALBEDO);
	mrt_assert(m, m->skybox.right, "loading skybox failed");
	append_paths(cwd, "skybox/skybox.up.xpm42", path);
	m->skybox.up = load_texture(m, path, ALBEDO);
	mrt_assert(m, m->skybox.up, "loading skybox failed");
	append_paths(cwd, "skybox/skybox.back.xpm42", path);
	m->skybox.back = load_texture(m, path, ALBEDO);
	mrt_assert(m, m->skybox.back, "loading skybox failed");
	append_paths(cwd, "skybox/skybox.down.xpm42", path);
	m->skybox.down = load_texture(m, path, ALBEDO);
	mrt_assert(m, m->skybox.down, "loading skybox failed");
	m->max_ray_bounces = DEFAULT_MAX_RAY_BOUNCES;
	m->disable_skybox = SKYBOX_DISABLED_BY_DEFAULT;
}
