/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aotsala <aotsala@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:39:51 by aotsala           #+#    #+#             */
/*   Updated: 2023/09/24 07:13:17 by aotsala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	find_dir_error(char *command, struct stat st)
{
	if (stat(command, &st) == 0)
	{
		if (S_ISDIR(st.st_mode))
		{
			n_errors(2, command, ": is a directory");
			exit(126);
		}
		if (access(command, X_OK) != 0)
		{
			n_errors(2, command, ": Permission denied");
			exit(126);
		}
	}
	n_errors(2, command, ": No such file or directory");
}

void	find_correct_error_msg(char	*command)
{
	int				i;
	int				dir;
	struct stat		st;

	dir = 0;
	i = 0;
	while (command[i])
	{
		if (command[i++] == '/')
			dir = 1;
	}
	if (dir)
		find_dir_error(command, st);
	else
		n_errors(2, command, ": command not found");
	exit(127);
}

void	errors(char *msg)
{
	int	i;

	i = 0;
	while (msg[i])
		i++;
	ft_putstr_fd("うーんシェル: ", 2);
	write(2, msg, i);
	exit(1);
}

void	n_errors(int n, ...)
{
	va_list	msg;

	ft_putstr_fd("うーんシェル: ", 2);
	va_start(msg, n);
	while (n > 0)
	{
		ft_putstr_fd(va_arg(msg, char *), 2);
		n--;
	}
	va_end(msg);
	ft_putchar_fd('\n', 2);
}

void	*parse_error(t_cmd *cmd, char *msg)
{
	int	i;

	i = 0;
	while (msg[i])
		i++;
	ft_putstr_fd("うーんシェル: ", 2);
	write(2, msg, i);
	ft_putchar_fd('\n', 2);
	g_exit_status = 258;
	free_cmd_tree(cmd);
	return (NULL);
}
