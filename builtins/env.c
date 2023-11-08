/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aotsala <aotsala@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:39:51 by aotsala           #+#    #+#             */
/*   Updated: 2023/09/16 00:08:29 by aotsala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_linked_list(t_env_st *env_st)
{
	t_env	*current;

	current = env_st->envlist;
	while (current != NULL)
	{
		printf("%s%s\n", current->var, current->val);
		current = current->next;
	}
}

int	env(char **args, t_env_st *env_st)
{
	if (args[1])
	{
		n_errors(3, "env: ", args[1], ": No such file or directory");
		return (1);
	}
	print_linked_list(env_st);
	return (0);
}
