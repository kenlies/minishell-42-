/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slampine <slampine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:39:51 by aotsala           #+#    #+#             */
/*   Updated: 2023/09/25 11:36:36 by slampine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_getenv(char *var, t_env_st *env_st)
{
	t_env	*node;
	char	*val;

	node = locate_env_var(var, env_st);
	if (!node)
		return (NULL);
	val = node->val;
	val++;
	return (val);
}

char	*get_var_val(char *var_str, int mode)
{
	int		i;
	int		k;
	char	*ret;

	i = 0;
	if (mode == 1)
	{
		while (var_str[i] != '=' && var_str[i])
			i++;
		ret = ft_substr(var_str, 0, i);
		return (ret);
	}
	else if (mode == 2)
	{
		while (var_str[i] != '=' && var_str[i])
			i++;
		k = i;
		while (var_str[i])
			i++;
		ret = ft_substr(var_str, k, i);
		return (ret);
	}
	return (NULL);
}

int	valid_var_name(char *arg)
{
	if (!ft_isalpha(*arg) && *arg != '_')
		return (0);
	arg++;
	while (*arg)
	{
		if (!ft_isalnum(*arg) && *arg != '_')
			return (0);
		arg++;
	}
	return (1);
}

void	remove_env_var(t_env *remove, t_env_st *env_st)
{
	t_env	*lst;
	t_env	*prev;

	lst = env_st->envlist;
	prev = NULL;
	while (lst)
	{
		if (lst == remove)
		{
			if (prev)
				prev->next = lst->next;
			else
				env_st->envlist = env_st->envlist->next;
			free(remove->var);
			free(remove->val);
			free(remove);
			return ;
		}
		prev = lst;
		lst = lst->next;
	}
}

t_env	*locate_env_var(char *var, t_env_st *env_st)
{
	t_env	*tmp;
	char	*envlist_var;

	tmp = env_st->envlist;
	if (tmp == NULL)
		return (NULL);
	envlist_var = tmp->var;
	while (tmp && ft_strcmp(envlist_var, var))
	{
		tmp = tmp->next;
		if (tmp)
			envlist_var = tmp->var;
	}
	return (tmp);
}
