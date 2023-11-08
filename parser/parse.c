/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aotsala <aotsala@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:39:51 by aotsala           #+#    #+#             */
/*   Updated: 2023/09/24 07:19:49 by aotsala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*par_exec(char **ps, char *es, int brackets)
{
	t_execcmd	*cmd;
	t_cmd		*ret;

	if (next(ps, es, "("))
		return (par_brackets(ps, es, brackets));
	ret = exec_cmd();
	cmd = (t_execcmd *)ret;
	ret = par_redirs(ret, ps, es);
	if (ret == NULL)
		return (NULL);
	ret = par_exec_loop(ps, es, cmd, ret);
	return (ret);
}

t_cmd	*par_redirs(t_cmd *cmd, char **ps, char *es)
{
	int		tok;
	char	*q;
	char	*eq;

	while (next(ps, es, "<>"))
	{
		tok = tokenizer(ps, es, 0, 0);
		if (tokenizer(ps, es, &q, &eq) != 'a')
			return (parse_error(cmd, "syntax error near redirection"));
		if (tok == '<')
			cmd = redir_cmd(cmd, q, eq, O_RDONLY);
		else if (tok == '>')
			cmd = redir_cmd(cmd, q, eq, O_WRONLY | O_CREAT | O_TRUNC);
		else if (tok == '+')
			cmd = redir_cmd(cmd, q, eq, O_WRONLY | O_CREAT | O_APPEND);
		else if (tok == '-')
		{
			do_heredoc(q, eq);
			cmd = redir_cmd(cmd, "heredoc.txt", NULL, O_RDONLY);
		}
	}
	return (cmd);
}

t_cmd	*par_bonus(t_cmd *cmd, char **ps, char *es, int brackets)
{
	int		tok;
	t_cmd	*tmp;

	tok = tokenizer(ps, es, 0, 0);
	if (tok == '@')
	{
		tmp = par_line(ps, es, brackets);
		if (!tmp)
		{
			free_cmd_tree(cmd);
			return (NULL);
		}
		cmd = bonus_cmd(cmd, tmp, 0, brackets);
	}
	else if (tok == '!')
	{
		tmp = par_line(ps, es, brackets);
		if (!tmp)
		{
			free_cmd_tree(cmd);
			return (NULL);
		}
		cmd = bonus_cmd(cmd, tmp, 1, brackets);
	}
	return (cmd);
}

t_cmd	*par_line(char **ps, char *es, int brackets)
{
	t_cmd	*cmd;
	t_cmd	*tmp;
	int		tok;

	cmd = par_pipe(ps, es, brackets);
	if (cmd == NULL)
		return (NULL);
	if (next_bonus(ps, es, "&", "|"))
		return (par_bonus(cmd, ps, es, brackets));
	return (cmd);
}

t_cmd	*par_pipe(char **ps, char *es, int brackets)
{
	t_cmd	*cmd;

	cmd = par_exec(ps, es, brackets);
	if (cmd == NULL)
	{
		return (NULL);
	}
	if (next(ps, es, "|") && (!check_for_oror(ps)))
	{
		tokenizer(ps, es, 0, 0);
		if (next(ps, es, "\0"))
			return (parse_error(cmd, "syntax error near pipe"));
		cmd = pipe_cmd(cmd, par_pipe(ps, es, brackets));
	}
	return (cmd);
}
