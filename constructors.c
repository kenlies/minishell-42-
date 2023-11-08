/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constructors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aotsala <aotsala@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:39:51 by aotsala           #+#    #+#             */
/*   Updated: 2023/09/24 07:35:47 by aotsala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*exec_cmd(void)
{
	t_execcmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
		errors("うーんシェル: fatal error mallocing exec_cmd: exiting\n");
	memset(cmd, 0, sizeof(*cmd));
	cmd->type = EXEC;
	return ((t_cmd *)cmd);
}

t_cmd	*redir_cmd(t_cmd *subcmd, char *file, char *efile, int mode)
{
	t_redircmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
		errors("うーんシェル: fatal error mallocing reidr_cmd: exiting\n");
	cmd->type = REDIR;
	cmd->cmd = subcmd;
	cmd->file = file;
	cmd->efile = efile;
	cmd->mode = mode;
	if (!ft_strcmp(file, "heredoc.txt"))
		cmd->type = HEREDOC;
	if (mode == 0 && ft_strcmp(file, "heredoc.txt"))
		cmd->type = REDIR_IN;
	if (mode == 0)
		cmd->fd = 0;
	else
		cmd->fd = 1;
	return ((t_cmd *)cmd);
}

t_cmd	*pipe_cmd(t_cmd *left, t_cmd *right)
{
	t_pipecmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
		errors("うーんシェル: fatal error mallocing pipe_cmd: exiting\n");
	cmd->type = PIPE;
	cmd->left = left;
	cmd->right = right;
	return ((struct s_cmd *)cmd);
}

t_cmd	*bonus_cmd(t_cmd *left, t_cmd *right, int type, int brackets)
{
	t_bonuscmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
		errors("うーんシェル: fatal error mallocing bonus_cmd: exiting\n");
	if (type == 0)
		cmd->type = ANDAND;
	else if (type == 1)
		cmd->type = OROR;
	cmd->left = left;
	cmd->right = right;
	cmd->brackets = brackets;
	return ((t_cmd *)cmd);
}
