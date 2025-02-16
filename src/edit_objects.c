// TODO stdheader

#include "minirt.h"
#include <math.h>

static void	edit_common_shape_attributes(t_minirt *m, t_shape *s, double x)
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
	edit_common_shape_attributes(m, s, x);
	if (m->moving_slider == 5)
		((t_sphere *)s)->radius = SCALE * x;
}

static void	edit_plane(t_minirt *m, t_shape *plane, double val)
{
	double	t;
	t_vec3	*n;

	edit_common_shape_attributes(m, plane, val);
	val = 2 * val - 1;
	n = &((t_plane *)plane)->normal;
	if (fabs(val) == 1 || fabs(n->x) == 1 || fabs(n->y) == 1 || fabs(n->z) == 1)
	{
		t = sqrt((1 - val * val) / 2);
		n->x = (m->moving_slider == 5) * val + (m->moving_slider != 5) * t;
		n->y = (m->moving_slider == 6) * val + (m->moving_slider != 6) * t;
		n->z = (m->moving_slider == 7) * val + (m->moving_slider != 7) * t;
		return ;
	}
	if (m->moving_slider == 5)
		t = sqrt((1 - val * val) / (n->y * n->y + n->z * n->z));
	else if (m->moving_slider == 6)
		t = sqrt((1 - val * val) / (n->x * n->x + n->z * n->z));
	else
		t = sqrt((1 - val * val) / (n->x * n->x + n->y * n->y));
	n->x = (m->moving_slider == 5) * val + (m->moving_slider != 5) * t * n->x;
	n->y = (m->moving_slider == 6) * val + (m->moving_slider != 6) * t * n->y;
	n->z = (m->moving_slider == 7) * val + (m->moving_slider != 7) * t * n->z;
}

static void	edit_cylinder(t_minirt *m, t_shape *cylinder, double x)
{
	t_cylinder	*c;
	size_t		i;

	edit_plane(m, cylinder, x);
	c = (t_cylinder *)cylinder;
	if (m->moving_slider == 8)
		c->radius = SCALE * x;
	if (m->moving_slider == 9)
		c->height = SCALE * x;
	i = 2 * (c - m->cylinders);
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

static void	edit_global_attributes(t_minirt *m, double x)
{
	if (m->moving_slider == 1)
		m->ambient_light_ratio = x;
	if (m->moving_slider == 2)
		m->ambient_light_color.r = x;
	if (m->moving_slider == 3)
		m->ambient_light_color.g = x;
	if (m->moving_slider == 4)
		m->ambient_light_color.b = x;
	if (m->moving_slider == 5)
		m->camera_field_of_view = 180 * x;
	if (1 <= m->moving_slider && m->moving_slider <= 4)
		m->ambient_light = vec3_muls( \
			m->ambient_light_color, m->ambient_light_ratio);
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
	if (m->shape_type == SHAPE_GLOBAL_ATTRIBUTES)
		edit_global_attributes(m, x);
	redraw(m);
}
