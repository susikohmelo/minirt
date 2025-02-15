/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 11:04:50 by lfiestas          #+#    #+#             */
/*   Updated: 2025/02/12 21:26:36 by ljylhank         ###   ########.fr       */
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

char	*f_to_str(char buf[static 32], double f)
{
	int64_t	i;

	i = f;
	ft_i_to_str(buf, i);
	if (fabs(f - i) < .0001)
		return (buf);
	if (-1 < f && f < 0)
		buf[0] = '-';
	if (f < 0)
	{
		f = -f;
		i = -i;
	}
	ft_i_to_str(ft_memcpy(buf + ft_strlen(buf), ".", 2) + 1, (f - i) * 10000);
	while (buf[ft_strlen(buf) - 1] != '.' && buf[ft_strlen(buf) - 1] == '0')
		buf[ft_strlen(buf) - 1] = '\0';
	if (buf[ft_strlen(buf) - 1] == '.')
		buf[ft_strlen(buf) - 1] = '\0';
	return (buf);
}

char	*trim_left(const char *str)
{
	while (ft_isspace(*str))
		++str;
	return ((char *)str);
}

bool	expect_range(t_minirt *m, t_vec3 inputs, const char *name)
{
	char	msg[256];
	char	buf[32];
	double	min;
	double	max;

	min = inputs.y;
	max = inputs.z;
	ft_strlcpy(msg, name, sizeof msg);
	ft_strlcat(msg, " not in range [", sizeof msg);
	ft_strlcat(msg, f_to_str(buf, min), sizeof msg);
	ft_strlcat(msg, ",", sizeof msg);
	ft_strlcat(msg, f_to_str(buf, max), sizeof msg);
	ft_strlcat(msg, "]", sizeof msg);
	mrt_expect(m, min <= inputs.x && inputs.x <= max, msg);
	return (true);
}

t_vec3	expect_normalized(t_minirt *m, t_vec3 v, const char *name)
{
	char	msg[256];

	ft_strlcpy(msg, "Unnormalized ", sizeof msg);
	ft_strlcat(msg, name, sizeof msg);
	mrt_expect(m, fabs(vec3_length(v) - 1.) < .001, msg);
	return (vec3_normalize(v));
}
