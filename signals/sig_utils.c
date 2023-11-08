/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aotsala <aotsala@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:39:51 by aotsala           #+#    #+#             */
/*   Updated: 2023/09/24 07:33:43 by aotsala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handler(int signal)
{
	if (signal == SIGINT)
	{
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_exit_status = 1;
	}
}

void	handler_child(int signal)
{
	if (signal == SIGINT)
		g_exit_status = 130;
	else if (signal == SIGQUIT)
		g_exit_status = 131;
}

void	handler_heredoc(int signal)
{
	char	b[2];

	if (signal == SIGINT)
		g_exit_status = 1;
	b[0] = 4;
	b[1] = 0;
	ioctl(STDIN_FILENO, TIOCSTI, b);
}

void	close_echoctl(struct termios *t)
{
	t->c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, t);
}

void	reset_echoctl(struct termios *t)
{
	t->c_lflag |= ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, t);
}
