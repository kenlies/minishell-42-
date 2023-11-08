/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aotsala <aotsala@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:39:51 by aotsala           #+#    #+#             */
/*   Updated: 2023/09/24 07:39:25 by aotsala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	add_to_env_list(char *var, char *val, t_env_st *env_st)
{
	t_env	*new;

	new = ft_lstnew_env(var, val);
	if (new)
	{
		ft_lstadd_back_env(&env_st->envlist, new);
		return (1);
	}
	return (0);
}

int	modify_env_list(t_env *remove, char *var, char *val, t_env_st *env_st)
{
	if (val[0])
	{
		if (!(add_to_env_list(var, val, env_st)))
		{
			n_errors(1, "export: error adding variable to envlist");
			free(var);
			free(val);
			return (1);
		}
		if (remove)
			remove_env_var(remove, env_st);
		free_and_update_env_list_array(env_st);
	}
	else
	{
		free(var);
		free(val);
	}
	return (0);
}

int	export(char **args, t_env_st *env_st)
{
	char	*var;
	char	*val;
	int		ret;

	ret = 0;
	if (!args[1])
		print_linked_list(env_st);
	args++;
	while (*args)
	{
		var = get_var_val(*args, 1);
		val = get_var_val(*args, 2);
		if (valid_var_name(var))
			ret |= modify_env_list
				(locate_env_var(var, env_st), var, val, env_st);
		else
		{
			n_errors(3, "export: `", *args, "': not a valid identifier");
			free(var);
			free(val);
			ret = 1;
		}
		args++;
	}
	return (ret);
}
