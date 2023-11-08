/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aotsala <aotsala@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:39:51 by aotsala           #+#    #+#             */
/*   Updated: 2023/09/16 01:40:26 by aotsala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	unset(char **args, t_env_st *env_st)
{
	int		ret;
	char	*var;
	t_env	*replace;

	ret = 0;
	while (*args)
	{
		var = get_var_val(*args, 1);
		if (valid_var_name(*args))
		{
			replace = locate_env_var(var, env_st);
			if (replace)
				remove_env_var(replace, env_st);
			free_and_update_env_list_array(env_st);
		}
		else
		{
			n_errors(3, "unset: `", *args, "': not a valid identifier");
			ret = 1;
		}
		free(var);
		args++;
	}
	return (ret);
}
