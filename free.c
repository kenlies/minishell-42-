/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aotsala <aotsala@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:39:51 by aotsala           #+#    #+#             */
/*   Updated: 2023/09/25 05:48:08 by aotsala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cmd_tree(t_cmd *cmd)
{
	if (cmd == 0)
		return ;
	if (cmd->type == EXEC)
		free(cmd);
	else if (cmd->type == REDIR || cmd->type == REDIR_IN
		|| cmd->type == HEREDOC)
	{
		free_cmd_tree(((t_redircmd *)cmd)->cmd);
		free(cmd);
	}
	else if (cmd->type == PIPE)
	{
		free_cmd_tree(((t_pipecmd *)cmd)->left);
		free_cmd_tree(((t_pipecmd *)cmd)->right);
		free(cmd);
	}
	else if (cmd->type == ANDAND || cmd->type == OROR)
	{
		free_cmd_tree(((t_bonuscmd *)cmd)->left);
		free_cmd_tree(((t_bonuscmd *)cmd)->right);
		free(cmd);
	}
}

void	free_double_pointer(char **argv)
{
	int	i;

	i = 0;
	if (!argv)
		return ;
	while (argv[i])
		free(argv[i++]);
	free(argv);
}

void	free_and_update_env_list_array(t_env_st *env_st)
{
	int	i;

	i = 0;
	while (env_st->envp_upd[i])
		free(env_st->envp_upd[i++]);
	free(env_st->envp_upd);
	env_st->envp_upd = update_envp(env_st);
}
