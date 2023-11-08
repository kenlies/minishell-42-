/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aotsala <aotsala@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:39:51 by aotsala           #+#    #+#             */
/*   Updated: 2023/09/24 07:11:25 by aotsala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_exec(t_cmd *cmd)
{
	char		*path;
	char		**q_argv;
	t_execcmd	*ecmd;
	int			i;

	ecmd = (t_execcmd *)cmd;
	printf("q_argv: ");
	i = 0;
	while (ecmd->argv[i])
		printf("%s ", ecmd->argv[i++]);
	printf("\n");
}

void	print_redir(t_cmd *cmd, int ind)
{
	t_redircmd	*rcmd;

	rcmd = (t_redircmd *)cmd;
	if (rcmd->mode == 1537)
		printf("| redirecting output to | ");
	else if (rcmd->mode == 0 && ft_strcmp(rcmd->file, "heredoc.txt"))
		printf("| redirecting input from | ");
	else if (rcmd->mode == 521)
		printf("| redirecting output in append mode to | ");
	else if (rcmd->mode == 0 && !ft_strcmp(rcmd->file, "heredoc.txt"))
		printf("HEREDOC ");
	ind++;
	printf("filename: %s \n", rcmd->file);
	print_tree(rcmd->cmd, ind);
}

void	print_pipe(t_cmd *cmd)
{
	t_pipecmd	*pcmd;
	static int	ind = 0;

	pcmd = (t_pipecmd *)cmd;
	ind++;
	print_tabs(ind);
	printf("Fork 1 (left cmd):\n");
	print_tree(pcmd->left, ind);
	print_tabs(ind);
	printf("Fork 2 (right cmd):\n");
	print_tree(pcmd->right, ind);
	ind = 0;
}

void	print_tabs(int ind)
{
	while (ind)
	{
		printf("	");
		ind--;
	}
}

void	print_tree(t_cmd *cmd, int ind)
{
	static int	counter = 0;

	if (cmd->type == EXEC)
	{
		counter++;
		print_tabs(ind);
		printf("("US""BW"%d"WHT""UR") "BBL"EXEC::"WHT" ", counter);
		print_exec(cmd);
	}
	else if (cmd->type == REDIR || cmd->type == REDIR_IN
		|| cmd->type == HEREDOC)
	{
		counter++;
		print_tabs(ind);
		printf("("US""BW"%d"WHT""UR") "BR"REDIR::"WHT" ", counter);
		print_redir(cmd, ind);
	}
	else if (cmd->type == PIPE)
	{
		counter++;
		print_tabs(ind);
		printf("("US""BW"%d"WHT""UR") "BM"PIPE::"WHT" \n", counter);
		print_pipe(cmd);
	}
}
