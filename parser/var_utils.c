/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aotsala <aotsala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:39:51 by aotsala           #+#    #+#             */
/*   Updated: 2023/09/27 13:30:20 by aotsala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	expand_dq_dol_q_mark(int *i, int *k, char **ret, char **str)
{
	char	*val;

	val = NULL;
	if ((*str)[*i] == '?')
	{
		(*i)++;
		val = ft_itoa(g_exit_status);
		copy_to_ret(&(*ret), val, k);
		free(val);
		return (1);
	}
	return (0);
}

void	copy_to_ret(char **ret, char *val, int *index)
{
	int	i;

	i = 0;
	while (val[i])
		(*ret)[(*index)++] = val[i++];
}

char	*create_exp_var(char *dollar_p)
{
	int			i;
	char		*ret;

	i = 0;
	while (dollar_p[i] && (ft_isalnum(dollar_p[i])
			|| dollar_p[i] == '_' || dollar_p[i] == '!'))
		i++;
	ret = malloc(sizeof(char) * i + 1);
	if (!ret)
		errors("うーんシェル: fatal error mallocing in create_exp_var: exiting\n");
	ft_strncpy(ret, dollar_p, i);
	ret[i] = '\0';
	return (ret);
}

int	is_equal(char *argv)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		if (argv[i] == '"')
		{
			i++;
			while (argv[i] != '"' && argv[i])
				i++;
			if (argv[i++] == '\0')
				return (0);
		}
		else if (argv[i] == '\'')
		{
			i++;
			while (argv[i] != '\'' && argv[i])
				i++;
			if (argv[i++] == '\0')
				return (0);
		}
		else
			i++;
	}
	return (1);
}

char	**is_equal_quote_count(char **argv)
{
	int		i;
	char	**ret;

	i = 0;
	while (argv[i])
	{
		if (!is_equal(argv[i]))
			break ;
		i++;
		if (argv[i] == NULL)
			return (NULL);
	}
	i = 0;
	while (argv[i])
		i++;
	ret = malloc(sizeof(char **) * (i + 1));
	if (!ret)
		errors("うーんシェル: fatal error mallocing var equal quote: exiting\n");
	return (stupid_ahh_function_for_norm(ret, argv));
}
