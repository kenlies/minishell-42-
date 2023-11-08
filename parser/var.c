/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aotsala <aotsala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:39:51 by aotsala           #+#    #+#             */
/*   Updated: 2023/09/27 13:31:05 by aotsala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	expand_dq(t_incs *incs, char **ret, char **str, t_env_st *env_st)
{
	char	*val;

	val = NULL;
	(*ret)[incs->k] = (*str)[incs->i];
	increment(&incs->i, &incs->k);
	while ((*str)[incs->i] != '"' && (*str)[incs->i])
	{
		if ((*str)[incs->i] == '$' && ++incs->i)
		{
			if (expand_dq_dol_q_mark(&incs->i, &incs->k, ret, str))
				continue ;
			find_exp_val_len_ret_val(*str, incs->i, &val, env_st);
			if (val)
				copy_to_ret(&(*ret), val, &incs->k);
			val = NULL;
			while ((*str)[incs->i] && (ft_isalnum((*str)[incs->i])
				|| (*str)[incs->i] == '_' || (*str)[incs->i] == '!'))
				incs->i++;
			continue ;
		}
		(*ret)[incs->k] = (*str)[incs->i];
		increment(&incs->i, &incs->k);
	}
	(*ret)[incs->k] = (*str)[incs->i];
	increment(&incs->i, &incs->k);
}

void	expand_sq(t_incs *incs, char **ret, char **str)
{
	(*ret)[incs->k] = (*str)[incs->i];
	increment(&incs->i, &incs->k);
	while ((*str)[incs->i] != '\'' && (*str)[incs->i])
	{
		(*ret)[incs->k] = (*str)[incs->i];
		increment(&incs->i, &incs->k);
	}
	(*ret)[incs->k] = (*str)[incs->i];
	increment(&incs->i, &incs->k);
}

void	expand_dol(t_incs *incs, char **ret, char **str, t_env_st *env_st)
{
	char	*val;

	val = NULL;
	incs->i++;
	if ((*str)[incs->i] == '?')
	{
		incs->i++;
		val = ft_itoa(g_exit_status);
		copy_to_ret(&(*ret), val, &incs->k);
		free(val);
		return ;
	}
	find_exp_val_len_ret_val(*str, incs->i, &val, env_st);
	if (val)
		copy_to_ret(&(*ret), val, &incs->k);
	val = NULL;
	while ((*str)[incs->i] && (ft_isalnum((*str)[incs->i]) 
		|| (*str)[incs->i] == '_' || (*str)[incs->i] == '!'))
		incs->i++;
}

char	*expand(char *str, t_env_st *env_st)
{
	char	*ret;
	t_incs	incs;
	int		var_len;

	var_len = get_var_len(str, env_st);
	ret = malloc(sizeof(char) * (var_len + 1));
	if (!ret)
		errors("うーんシェル: fatal error mallocing in expand: exiting\n");
	ft_memset(&incs, 0, sizeof(t_incs));
	while (str[incs.i] != '\0')
	{
		if (str[incs.i] == '"')
			expand_dq(&incs, &ret, &str, env_st);
		else if (str[incs.i] == '\'')
			expand_sq(&incs, &ret, &str);
		else if (str[incs.i] == '$' && str[incs.i + 1])
			expand_dol(&incs, &ret, &str, env_st);
		else
		{
			ret[incs.k] = str[incs.i];
			increment(&incs.i, &incs.k);
		}
	}
	ret[incs.k] = '\0';
	return (ret);
}

char	**process_vars(char **argv, t_env_st *env_st)
{
	char	**q_argv;
	char	**unclosed_quotes;
	char	*var;
	int		i;

	unclosed_quotes = is_equal_quote_count(argv);
	if (unclosed_quotes)
		return (unclosed_quotes);
	i = 0;
	while (argv[i])
		i++;
	q_argv = malloc(sizeof(char **) * (i + 1));
	if (!q_argv)
		errors("うーんシェル: fatal error mallocing in process_vars: exiting\n");
	q_argv[i] = NULL;
	i = 0;
	while (argv[i])
	{
		var = expand(argv[i], env_st);
		q_argv[i] = var;
		i++;
	}
	return (q_argv);
}
