/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbilyj <nbilyj@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 10:44:06 by nbilyj            #+#    #+#             */
/*   Updated: 2026/03/06 10:44:07 by nbilyj           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static int    is_digit(char *str)
{
	int    i;
	int    has_digit;

	i = 0;
	has_digit = 0;
	if (!str || !str[0])
		return (0);
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		has_digit = 1;
		i++;
	}
	return (has_digit);
}

static long long    ft_atol(char *str)
{
	int            i;
	long long    result;
	long long    sign;

	result = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10;
		result = result + str[i] - '0';
		if (result > 2147483647)
			return (2147483648);
		i++;
	}
	return (result * sign);
}

int    safe_atoi(char *str)
{
	long long    value;

	if (!is_digit(str))
		return (-1);
	value = ft_atol(str);
	if (value > 2147483647 || value < 0)
		return (-1);
	return ((int)value);
}