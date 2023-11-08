/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slampine <slampine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:39:51 by aotsala           #+#    #+#             */
/*   Updated: 2023/09/27 11:27:16 by slampine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_builtin(char **args)
{
	if (!ft_strcmp(args[0], "cd")
		|| !ft_strcmp(args[0], "echo")
		|| !ft_strcmp(args[0], "env")
		|| !ft_strcmp(args[0], "exit")
		|| !ft_strcmp(args[0], "export")
		|| !ft_strcmp(args[0], "unset")
		|| !ft_strcmp(args[0], "pwd"))
		return (1);
	return (0);
}

int	run_builtin(char **args, t_env_st *env_st)
{
	int	ret;

	if (!ft_strcmp(args[0], "cd"))
		ret = cd(args, env_st);
	else if (!ft_strcmp(args[0], "echo"))
		ret = echo(args);
	else if (!ft_strcmp(args[0], "env"))
		ret = env(args, env_st);
	else if (!ft_strcmp(args[0], "exit"))
		ret = exit_mini(args);
	else if (!ft_strcmp(args[0], "export"))
		ret = export(args, env_st);
	else if (!ft_strcmp(args[0], "unset"))
		ret = unset(args, env_st);
	else if (!ft_strcmp(args[0], "pwd"))
		ret = pwd();
	else
		ret = 1;
	free_double_pointer(args);
	return (ret);
}
