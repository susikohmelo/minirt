/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 11:04:50 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/07 11:18:45 by lfiestas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <unistd.h>
#include <math.h>

double	str_to_f(const char *str)
{
	uint64_t	integer;
	uint64_t	fraction;
	size_t		fraction_power;
	double		sign;

	sign = 1.;
	if ((*str == '+' || *str == '-'))
		sign = 1. - 2 * (*str++ == '-');
	integer = 0;
	while (ft_isdigit(*str) && integer < UINT64_MAX / 10 - 10)
		integer = integer * 10 + *str++ - '0';
	if (integer >= UINT64_MAX / 10 - 10)
		return (sign * INFINITY);
	if (*str == '.')
		++str;
	fraction = 0;
	fraction_power = 1;
	while (ft_isdigit(*str) && fraction < (1llu << 52) - 1)
	{
		fraction_power *= 10;
		fraction = fraction * 10 + *str++ - '0';
	}
	return (sign * (integer + (double)fraction / fraction_power));
}

char	*trim_left(const char *str)
{
	while (ft_isspace(*str))
		++str;
	return ((char *)str);
}

bool	assert_range(t_minirt *m, t_vec3 inputs, const char *name)
{
	char	msg[256];
	char	buf[16];
	double	min;
	double	max;

	min = inputs.y;
	max = inputs.z;
	ft_strlcpy(msg, name, sizeof msg);
	ft_strlcat(msg, " not in range [", sizeof msg);
	ft_strlcat(msg, ft_i_to_str(buf, min), sizeof msg);
	ft_strlcat(msg, ",", sizeof msg);
	ft_strlcat(msg, ft_i_to_str(buf, max), sizeof msg);
	ft_strlcat(msg, "]", sizeof msg);
	mrt_assert(m, min <= inputs.x && inputs.x <= max, msg);
	return (true);
}

t_vec3	expect_normalized(t_vec3 v, const char *name)
{
	if (fabs(vec3_length(v) - 1.) >= .001)
	{
		ft_putstr_fd("Warning\nUnnormalized ", STDERR_FILENO);
		ft_putendl_fd(name, STDERR_FILENO);
	}
	return (vec3_normalize(v));
}
