/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aotsala <aotsala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:39:51 by aotsala           #+#    #+#             */
/*   Updated: 2023/09/27 13:31:47 by aotsala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_long_long(char *str)
{
	int					i;
	signed long long	res;
	long long			prev;
	int					sign;

	i = 0;
	res = 0;
	sign = 1;
	while (str[i] && ((str[i] >= 9 && str[i] <= 13) || str[i] == 32))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] != '\0' && str[i] >= '0' && str[i] <= '9')
	{
		prev = res;
		res = res * 10 + str[i] - '0';
		if (res < prev)
			return (1);
		i++;
	}
	return (0);
}

long long	ft_atoll(char *str)
{
	int			i;
	long long	res;
	int			sign;

	i = 0;
	res = 0;
	sign = 1;
	while (str[i] && ((str[i] >= 9 && str[i] <= 13) || str[i] == 32))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] != '\0' && str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + str[i] - '0';
		i++;
	}
	return (res * sign);
}

int	check_for_oror(char **ps)
{
	char		*pp;

	pp = *ps;
	if ((pp[0] == '|') && (pp[1] == '|'))
		return (1);
	else
		return (0);
}

int	is_history_worthy(char *cmdline)
{
	if (ft_strlen(cmdline) > 0)
	{
		while (*cmdline == ' ')
			cmdline++;
		if (*cmdline == '\0')
			return (0);
		return (1);
	}
	return (0);
}

char	**stupid_ahh_function_for_norm(char **ret, char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		ret[i] = ft_strdup(argv[i]);
		i++;
	}
	ret[i] = NULL;
	return (ret);
}
