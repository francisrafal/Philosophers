/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frafal <frafal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 15:12:02 by frafal            #+#    #+#             */
/*   Updated: 2023/01/26 11:19:17 by frafal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	unsigned char	*c1;
	unsigned char	*c2;

	if (n == 0)
		return (0);
	c1 = (unsigned char *)s1;
	c2 = (unsigned char *)s2;
	i = 0;
	if (c1[i] == '\0')
		return (c1[i] - c2[i]);
	while (c1[i] != '\0' && i < n)
	{
		if (c1[i] != c2[i])
			return (c1[i] - c2[i]);
		i++;
	}
	if (c1[i] == '\0' && i != n)
		return (c1[i] - c2[i]);
	return (0);
}

static int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

static int	over_llong(long long i, int sign, int digits, const char *nptr)
{
	char	*llong_max_abs;
	char	*llong_min_abs;

	llong_max_abs = "9223372036854775807";
	llong_min_abs = "9223372036854775808";
	if (sign == 1)
	{
		if (digits > 19)
			return (-1);
		if (digits == 19 && (ft_strncmp(&nptr[i - 19], llong_max_abs, 19) > 0))
			return (-1);
	}
	if (sign == -1)
	{
		if (digits > 19)
			return (0);
		if (digits == 19 && (ft_strncmp(&nptr[i - 19], llong_min_abs, 19) > 0))
			return (0);
	}
	return (1);
}

static void	trim_space_and_sign(long long *i, int *sign, const char *nptr)
{
	while (nptr[*i] == 32 || (nptr[*i] >= 9 && nptr[*i] <= 13))
		(*i)++;
	if (nptr[*i] == '-')
	{
		*sign = -1;
		(*i)++;
	}
	if (nptr[*i] == '+' && *sign != -1)
		(*i)++;
}

int	ft_atoi(const char *nptr)
{
	long long			i;
	int					sign;
	int					digits;
	unsigned long long	nb;

	i = 0;
	sign = 1;
	nb = 0;
	digits = 0;
	trim_space_and_sign(&i, &sign, nptr);
	while (nptr[i] != '\0' && ft_isdigit(nptr[i]))
	{
		nb = nb * 10 + (nptr[i++] - 48);
		digits++;
	}
	digits = over_llong(i, sign, digits, nptr);
	if (digits <= 0)
		return (digits);
	return (nb * sign);
}
