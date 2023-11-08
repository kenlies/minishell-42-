/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_redir_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aotsala <aotsala@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:39:51 by aotsala           #+#    #+#             */
/*   Updated: 2023/09/25 04:39:49 by aotsala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_path(char *cmd_str, t_env_st *env_st)
{
	char	*path;

	if (ft_strlen(cmd_str) == 0 || ft_strncmp(cmd_str, "..", 3) == 0
		|| ft_strncmp(cmd_str, ".", 2) == 0)
		path = NULL;
	else if (ft_strchr(cmd_str, '/'))
	{
		if (access(cmd_str, F_OK) == 0)
		{
			if (access(cmd_str, X_OK) == 0)
				return (ft_strdup(cmd_str));
			else
				return (NULL);
		}
		else
			path = NULL;
	}
	else
		path = build_cmd_str(cmd_str, env_st);
	return (path);
}

void	run_exec_handle_sys(char **q_argv, t_env_st *env_st)
{
	char	*path;
	pid_t	pid;

	path = get_path(q_argv[0], env_st);
	set_signal_child();
	pid = forky();
	if (pid == 0)
	{
		execve(path, q_argv, env_st->envp_upd);
		find_correct_error_msg(q_argv[0]);
		exit(1);
	}
	wait_child(pid);
	free(path);
	free_double_pointer(q_argv);
}

void	run_exec(t_cmd *cmd, t_env_st *env_st)
{
	char		**q_argv;
	t_execcmd	*ecmd;

	ecmd = (t_execcmd *)cmd;
	if (ecmd->argv[0] == 0)
		return ;
	q_argv = process_vars(ecmd->argv, env_st);
	q_argv = process_quotes(q_argv);
	if (q_argv == NULL)
		return ;
	q_argv = handle_wildcards(q_argv);
	if (is_builtin(q_argv) && env_st->bonus && env_st->do_brackets)
		g_exit_status = run_builtin(q_argv, env_st);
	else if (env_st->bonus && env_st->do_brackets)
		run_exec_handle_sys(q_argv, env_st);
	else
	{
		free_double_pointer(q_argv);
		env_st->bonus = 1;
	}
}

void	run_redir_continuation(t_redircmd *rcmd, t_redircmd *node,
			t_env_st *env_st)
{
	int			old_fd;
	int			fd;

	old_fd = dup(rcmd->fd);
	if (rcmd->file[0] == '$')
		return (ambiguous(rcmd, old_fd));
	fd = open(rcmd->file, rcmd->mode, 0664);
	if (fd < 0)
	{
		if (errno == EACCES)
			n_errors(2, rcmd->file, ": Permission denied");
		else
			n_errors(2, rcmd->file, ": No such file or directory");
		g_exit_status = 1;
		close(fd);
		close(old_fd);
		return ;
	}
	if (rcmd->type != HEREDOC)
		g_exit_status = 0;
	dup2(fd, rcmd->fd);
	run_cmd(node->cmd, env_st);
	dup2(old_fd, rcmd->fd);
	close(fd);
	close(old_fd);
}

void	run_redir(t_cmd *cmd, t_redircmd *carry, int flag, t_env_st *env_st)
{
	t_redircmd	*node;
	t_redircmd	*rcmd;

	node = (t_redircmd *)cmd;
	if ((node->cmd->type == REDIR && node->type == REDIR) 
		|| (node->cmd->type == REDIR_IN && node->type == REDIR_IN))
	{
		if (carry != NULL)
		{
			if (!create_dummy_file(node))
				return ;
			else
				run_redir(node->cmd, carry, 1, env_st);
		}
		else
			run_redir(node->cmd, node, 0, env_st);
		return ;
	}
	rcmd = carry;
	if (carry && (flag == 1 || node->type == REDIR))
		create_dummy_file(node);
	else
		rcmd = (t_redircmd *)cmd;
	run_redir_continuation(rcmd, node, env_st);
}
