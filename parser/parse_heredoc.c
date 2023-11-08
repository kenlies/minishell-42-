/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aotsala <aotsala@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:39:51 by aotsala           #+#    #+#             */
/*   Updated: 2023/09/24 07:22:42 by aotsala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*remove_quotes_from_delim(char *delimeter)
{
	char	*p;
	int		i;
	char	*ret;

	i = 0;
	p = delimeter;
	while (*p != '\0')
	{
		if (*p != '\'' && *p != '"')
			i++;
		p++;
	}
	p = delimeter;
	ret = malloc(sizeof(char) * i + 1);
	if (!ret)
		errors("うーんシェル: fatal error mallocing quotes heredoc: exiting\n");
	i = 0;
	while (*p != '\0')
	{
		if (*p != '\'' && *p != '"')
			ret[i++] = *p;
		p++;
	}
	ret[i] = '\0';
	return (ret);
}

char	*get_delim(char *beg_delim, char *end_delim)
{
	char	*p;
	int		i;
	char	*delimeter;
	char	*ret;

	i = end_delim - beg_delim;
	delimeter = malloc(sizeof(char) * i + 1);
	if (!delimeter)
		errors("うーんシェル: fatal error mallocing delim for heredoc: exiting\n");
	i = 0;
	p = beg_delim;
	while (p != end_delim)
	{
		delimeter[i] = *p;
		i++;
		p++;
	}
	delimeter[i] = '\0';
	ret = remove_quotes_from_delim(delimeter);
	free(delimeter);
	return (ret);
}

void	heredoc_buf(char **buf)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	set_signal_handlers_heredoc();
	close_echoctl(&term);
	*buf = readline("> ");
	reset_echoctl(&term);
}

void	free_and_close(char *delimeter, int fd)
{
	free(delimeter);
	close(fd);
}

void	do_heredoc(char *beg_delim, char *end_delim)
{
	char				*delimeter;
	int					fd;
	char				*buf;

	delimeter = get_delim(beg_delim, end_delim);
	buf = NULL;
	fd = open("heredoc.txt", O_CREAT | O_WRONLY | O_TRUNC, 0664);
	if (!fd)
		errors("うーんシェル: fatal error calling sys-cmd: open: exiting\n");
	heredoc_buf(&buf);
	if (buf == NULL)
		return (free_and_close(delimeter, fd));
	while (buf && ft_strcmp(buf, delimeter) != 0)
	{
		ft_putstr_fd(buf, fd);
		write(fd, "\n", 1);
		free(buf);
		heredoc_buf(&buf);
	}
	free(buf);
	free(delimeter);
	close(fd);
}
