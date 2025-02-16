// TODO stdheader

#include "minirt.h"
#include <math.h>

static void	edit_shape(t_minirt *m, t_shape *s, double x)
{
	if (m->moving_slider == 1)
		s->color.r = x;
	if (m->moving_slider == 2)
		s->color.g = x;
	if (m->moving_slider == 3)
		s->color.b = x;
	if (m->moving_slider == 4)
		s->default_rough = x;
}

static void	edit_sphere(t_minirt *m, t_shape *s, double x)
{
	edit_shape(m, s, x);
	if (m->moving_slider == 5)
		((t_sphere *)s)->radius = SCALE * x;
}

static void	edit_plane(t_minirt *m, t_shape *p, double value)
{
	edit_shape(m, p, value);
	value = 2 * value - 1;
	if (m->moving_slider == 5)
		((t_plane *)p)->normal.x = value;
	if (m->moving_slider == 6)
		((t_plane *)p)->normal.y = value;
	if (m->moving_slider == 7)
		((t_plane *)p)->normal.z = value;
	if (5 <= m->moving_slider && m->moving_slider <= 7)
		((t_plane *)p)->normal = vec3_normalize(((t_plane *)p)->normal);
}

static void	edit_cylinder(t_minirt *m, t_shape *cylinder, double x)
{
	t_cylinder *c;
	size_t		i;

	edit_plane(m, cylinder, x);
	c = (t_cylinder *)cylinder;
	if (m->moving_slider == 8)
		c->radius = SCALE * x;
	if (m->moving_slider == 9)
		c->height = SCALE * x;
	i = c - m->cylinders;
	m->discs[i].coords = vec3_add(c->coords, vec3_muls(c->axis, c->height / 2));
	m->discs[i].color = c->color;
	m->discs[i].default_rough = c->default_rough;
	m->discs[i].normal = c->axis;
	m->discs[i].radius = c->radius;
	++i;
	m->discs[i].coords = vec3_sub(c->coords, vec3_muls(c->axis, c->height / 2));
	m->discs[i].color = c->color;
	m->discs[i].default_rough = c->default_rough;
	m->discs[i].normal = c->axis;
	m->discs[i].radius = c->radius;
}

static void	edit_light(t_minirt *m, t_light *light, double x)
{
	if (m->moving_slider == 1)
		light->brightness = x;
	if (m->moving_slider == 2)
		light->color_value.r = x;
	if (m->moving_slider == 3)
		light->color_value.g = x;
	if (m->moving_slider == 4)
		light->color_value.b = x;
	light->color = vec3_muls(light->color_value, light->brightness);
}

void	edit_objects(t_minirt *m, double x)
{
	x = fmin(fmax(x / (LINE_LENGTH * CHAR_WIDTH), 0), 1);
	if (m->shape_type == SHAPE_SPHERE)
		edit_sphere(m, m->shape, x);
	if (m->shape_type == SHAPE_PLANE)
		edit_plane(m, m->shape, x);
	if (m->shape_type == SHAPE_CYLINDER)
		edit_cylinder(m, m->shape, x);
	if (m->shape_type == SHAPE_LIGHT)
		edit_light(m, (t_light *)m->shape, x);
	redraw(m);
}
