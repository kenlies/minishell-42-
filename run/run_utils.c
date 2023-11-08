/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slampine <slampine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:39:51 by aotsala           #+#    #+#             */
/*   Updated: 2023/09/25 13:29:51 by slampine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	forky(void)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		errors("うーんシェル: fatal error forking: exiting\n");
	return (pid);
}

void	wait_child(pid_t pid)
{
	int	e_status;

	if (waitpid(pid, &e_status, 0) != -1)
	{
		if (WIFEXITED(e_status))
			g_exit_status = WEXITSTATUS(e_status);
		if (WTERMSIG(e_status))
		{
			if (WTERMSIG(e_status) == SIGSEGV)
				ft_putendl_fd("Segmentation fault: 11", 2);
			else if (WTERMSIG(e_status) == SIGQUIT)
				ft_putendl_fd("Quit: 3", 2);
			g_exit_status = 128 + WTERMSIG(e_status);
		}
	}
}

void	ambiguous(t_redircmd *rcmd, int old_fd)
{
	n_errors(2, rcmd->file, ": ambiguous redirect");
	close(old_fd);
	return ;
}

int	create_dummy_file(t_redircmd *dummy)
{
	int		fd;

	if (dummy->mode == (O_WRONLY | O_CREAT | O_APPEND))
		fd = open(dummy->file, O_CREAT | O_RDWR | O_APPEND, 0664);
	else if (dummy->mode == (O_RDONLY) && ft_strcmp(dummy->file, "heredoc.txt"))
		fd = open(dummy->file, O_RDONLY, 0664);
	else
		fd = open(dummy->file, O_CREAT | O_RDWR | O_TRUNC, 0664);
	if (fd < 0)
	{
		if (errno == EACCES)
			n_errors(2, dummy->file, ": Permission denied");
		else
			n_errors(2, dummy->file, ": No such file or directory");
		g_exit_status = 1;
		close(fd);
		return (0);
	}
	g_exit_status = 0;
	close(fd);
	return (1); 
}

char	*build_cmd_str(char *cmd_str, t_env_st *env_st)
{
	char	**path_arr;
	char	*path;
	int		i;

	i = 0;
	path = NULL;
	path_arr = ft_split(ft_getenv("PATH", env_st), ':');
	while (path_arr && path_arr[i])
	{
		path = ft_strnjoin(3, path_arr[i], "/", cmd_str);
		if (!path || (access(path, F_OK) == 0 && access(path, X_OK) == 0))
			break ;
		free(path);
		path = NULL;
		i++;
	}
	free_double_pointer(path_arr);
	return (path);
}
