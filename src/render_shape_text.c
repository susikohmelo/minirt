/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_shape_text.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 17:45:13 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/19 12:54:35 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	render_common_shape_text(t_minirt *m)
{
	render_value(m, "Red", m->shape->color.r, 255);
	render_value(m, "Green", m->shape->color.g, 255);
	render_value(m, "Blue", m->shape->color.b, 255);
	render_value(m, "Roughness", m->shape->roughness, 1);
}

void	render_sphere_text(t_minirt *m)
{
	render_header(m, "Sphere");
	render_common_shape_text(m);
	render_value(m, "Radius", ((t_sphere *)m->shape)->radius / SCALE, SCALE);
}

void	render_plane_text(t_minirt *m)
{
	render_header(m, "Plane");
	render_common_shape_text(m);
	render_normalized_vector_value(m, "Normal", ((t_plane *)m->shape)->normal);
}

void	render_cylinder_text(t_minirt *m)
{
	render_header(m, "Cylinder");
	render_common_shape_text(m);
	render_normalized_vector_value(m, "Axis", ((t_cylinder *)m->shape)->axis);
	render_value(m, "Radius", ((t_cylinder *)m->shape)->radius / SCALE, SCALE);
	render_value(m, "Height", ((t_cylinder *)m->shape)->height / SCALE, SCALE);
}
