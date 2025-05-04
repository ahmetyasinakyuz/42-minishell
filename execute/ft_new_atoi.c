/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_new_atoi.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 14:33:28 by aycami            #+#    #+#             */
/*   Updated: 2025/05/04 10:19:22 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_white_space(int i, const char *str)
{
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	return (i);
}

void	sign_control(int *sign, int *i, const char *str)
{
	*sign = 1;
	if (str[*i] == '+')
		i++;
	else if (str[*i] == '-')
	{
		*sign = -1;
		i++;
	}
}

int	ft_new_atoi(const char *str, int *flag)
{
	int					i;
	unsigned long long	result;
	int					sign;
	int					limit_digit;

	i = 0;
	result = 0;
	i = ft_white_space(i, str);
	sign_control(&sign, &i, str);
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (sign == 1)
			limit_digit = 7;
		else
			limit_digit = 8;
		if (result > 922337203685477580 || (result == 922337203685477580
				&& (str[i] - '0') > limit_digit))
		{
			*flag = -1;
			return (-1);
		}
		result = result * 10 + (str[i++] - '0');
	}
	return ((long long)(result * sign) % 256);
}
