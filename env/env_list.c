/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aotsala <aotsala@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:39:51 by aotsala           #+#    #+#             */
/*   Updated: 2023/09/24 04:23:55 by aotsala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**update_envp(t_env_st *env_st)
{
	char	**ret;
	t_env	*current;
	int		i;

	current = env_st->envlist;
	i = ft_lstsize_env(current);
	ret = malloc(sizeof(char *) * (i + 1));
	if (!ret)
		errors("うーんシェル: fatal error mallocing update_envp: exiting\n");
	ret[i] = NULL;
	i = 0;
	while (current)
	{
		ret[i] = ft_strjoin(current->var, current->val);
		current = current->next;
		i++;
	}
	return (ret);
}

char	**dublicate_env(t_env_st *env_st)
{
	char	**envp_cpy;
	int		count;
	int		i;

	count = 0;
	while (env_st->envp_og[count])
		count++;
	envp_cpy = malloc(sizeof(char *) * (count + 1));
	if (!envp_cpy)
		errors("うーんシェル: fatal error mallocing: exiting\n");
	envp_cpy[count] = NULL;
	i = 0;
	while (i < count)
	{
		envp_cpy[i] = ft_strdup(env_st->envp_og[i]);
		i++;
	}
	return (envp_cpy);
}

void	create_env_list(t_env_st *env_st)
{
	t_env	*tmp;
	char	**envp_cpy;
	int		i;

	env_st->envlist = NULL;
	envp_cpy = dublicate_env(env_st);
	i = 0;
	while (envp_cpy[i])
	{
		tmp = ft_lstnew_env(get_var_val(envp_cpy[i], 1),
				get_var_val(envp_cpy[i], 2));
		if (!tmp)
			errors("うーんシェル: fatal error mallocing env_list: exiting\n");
		ft_lstadd_back_env(&env_st->envlist, tmp);
		i++;
	}
	i = 0;
	while (envp_cpy[i])
		free(envp_cpy[i++]);
	free(envp_cpy);
}
