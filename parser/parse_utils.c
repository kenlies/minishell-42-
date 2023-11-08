/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aotsala <aotsala@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:39:51 by aotsala           #+#    #+#             */
/*   Updated: 2023/09/25 08:07:39 by aotsala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*par_exec_loop(char **ps, char *es, t_execcmd *cmd, t_cmd *ret)
{
	int		tok;
	int		argc;
	char	*q;
	char	*eq;

	argc = 0;
	while (!next(ps, es, "|&)"))
	{
		tok = tokenizer(ps, es, &q, &eq);
		if (tok == 0)
			break ;
		ret = par_redirs(ret, ps, es);
		if (ret == NULL)
			return (NULL);
		cmd->argv[argc] = q;
		cmd->eargv[argc++] = eq;
		if (argc >= MAXARGS)
			return (parse_error(ret, "error: too many arguments"));
		ret = par_redirs(ret, ps, es);
		if (ret == NULL)
			return (NULL);
	}
	cmd->argv[argc] = 0;
	cmd->eargv[argc] = 0;
	return (ret);
}

t_cmd	*par_brackets(char **ps, char *es, int brackets)
{
	t_cmd	*cmd;

	if (!next(ps, es, "("))
		errors("うーんシェル: error in par_block\n");
	tokenizer(ps, es, 0, 0);
	if (next(ps, es, ")"))
	{
		n_errors(1, "syntax error near unexpected token `('");
		return (NULL);
	}
	cmd = par_line(ps, es, brackets + 1);
	brackets = brackets - 1;
	if (!next(ps, es, ")"))
		return (parse_error(cmd, "syntax error near unexpected token `('"));
	tokenizer(ps, es, 0, 0);
	cmd = par_redirs(cmd, ps, es);
	return (cmd);
}

int	next_bonus(char **ps, char *es, char *toks1, char *toks2)
{
	const char	whitespace[] = " \t\r\n\v";
	char		*pp1;
	char		*pp2;

	pp1 = *ps;
	pp2 = *ps;
	while (*ps < es && ft_strchr(whitespace, **ps))
		(*ps)++;
	pp1++;
	if ((**ps && *pp1 && ft_strchr(toks1, **ps) && ft_strchr(toks1, *pp1)))
	{
		pp1++;
		return (**ps && *pp1 && !ft_strchr(toks1, *pp1));
	}
	pp2++;
	if (**ps && *pp2 && ft_strchr(toks2, **ps) && ft_strchr(toks2, *pp2))
	{
		pp2++;
		return (**ps && *pp2 && !ft_strchr(toks2, *pp2));
	}
	return (0);
}

int	next(char **ps, char *es, char *toks)
{
	const char	whitespace[] = " \t\r\n\v";

	while (*ps < es && ft_strchr(whitespace, **ps))
		(*ps)++;
	if (**ps == '\0' && toks[0] == '\0')
		return (1);
	return (**ps && ft_strchr(toks, **ps));
}

void	nulterminate(t_cmd *cmd)
{
	int	i;

	i = 0;
	if (cmd == 0)
		return ;
	if (cmd->type == EXEC)
		while (((t_execcmd *)cmd)->argv[i])
			*((t_execcmd *)cmd)->eargv[i++] = 0;
	else if (cmd->type == REDIR || cmd->type == HEREDOC
		|| cmd->type == REDIR_IN)
	{
		nulterminate(((t_redircmd *)cmd)->cmd);
		if (((t_redircmd *)cmd)->efile)
			*((t_redircmd *)cmd)->efile = 0;
	}
	else if (cmd->type == PIPE)
	{
		nulterminate(((t_pipecmd *)cmd)->left);
		nulterminate(((t_pipecmd *)cmd)->right);
	}
	else if (cmd->type == ANDAND || cmd->type == OROR)
	{
		nulterminate(((t_bonuscmd *)cmd)->left);
		nulterminate(((t_bonuscmd *)cmd)->right);
	}
}
