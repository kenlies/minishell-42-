/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_len.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aotsala <aotsala@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:39:51 by aotsala           #+#    #+#             */
/*   Updated: 2023/09/16 21:26:37 by aotsala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	find_exp_val_len_ret_val(char *str, int index, char **val, t_env_st *env_st)
{
	char	*exp_var;
	char	*vall;
	t_env	*var;
	int		len;

	str = str + index;
	exp_var = create_exp_var(str);
	var = locate_env_var(exp_var, env_st);
	free(exp_var);
	if (!var)
		return (0);
	vall = var->val;
	vall++;
	if (val)
		*val = vall;
	len = ft_strlen(vall);
	return (len);
}

int	find_exit_len(int exit_status)
{
	int	len;

	len = -1;
	if (exit_status < 0)
		exit_status = -exit_status;
	if (exit_status == 0)
		len = 1;
	else
	{
		while (exit_status > 0)
		{
			exit_status /= 10;
			len++;
		}
	}
	return (len);
}

int	handle_dq_len(int *i, int *len, char **str, t_env_st *env_st)
{
	int			exp_var_len;

	exp_var_len = 0;
	increment(i, len);
	while ((*str)[*i] != '"' && (*str)[*i])
	{
		if ((*str)[*i] == '$')
		{
			(*i)++;
			if ((*str)[*i] == '?')
				exp_var_len = exp_var_len + (find_exit_len(g_exit_status));
			else
				exp_var_len = exp_var_len
					+ find_exp_val_len_ret_val(*str, *i, NULL, env_st);
			while ((*str)[*i] && (ft_isalnum((*str)[*i])
				|| (*str)[*i] == '_' || (*str)[*i] == '!'))
				(*i)++;
			continue ;
		}
		increment(i, len);
	}
	increment(i, len);
	return (exp_var_len);
}

int	handle_dol_len(int *i, char **str, t_env_st *env_st)
{
	int			exp_var_len;

	(*i)++;
	if ((*str)[*i] == '?')
		return (find_exit_len(g_exit_status));
	exp_var_len = exp_var_len 
		+ find_exp_val_len_ret_val(*str, *i, NULL, env_st);
	while ((*str)[*i] && (ft_isalnum((*str)[*i]) 
		|| (*str)[*i] == '_' || (*str)[*i] == '!'))
		(*i)++;
	return (exp_var_len);
}

int	get_var_len(char *str, t_env_st *env_st)
{
	int	i;
	int	len;
	int	exp_var_len;

	i = 0;
	len = 0;
	exp_var_len = 0;
	while (str[i])
	{
		if (str[i] == '"')
			exp_var_len = exp_var_len + handle_dq_len(&i, &len, &str, env_st);
		else if (str[i] == '\'')
		{
			increment(&i, &len);
			while (str[i] != '\'' && str[i])
				increment(&i, &len);
			increment(&i, &len);
		}
		else if (str[i] == '$' && str[i + 1])
			exp_var_len = exp_var_len + handle_dol_len(&i, &str, env_st);
		else
			increment(&i, &len);
	}
	return (len + exp_var_len);
}
