/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aotsala <aotsala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:39:51 by aotsala           #+#    #+#             */
/*   Updated: 2023/09/27 13:39:05 by aotsala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	prompt_exit(void)
{
	ft_putendl_fd("exit", 2);
	exit(g_exit_status);
}

void	run_cmd(t_cmd *cmd, t_env_st *env_st)
{
	int	p[2];

	if (cmd == 0)
		exit(1);
	if (cmd->type == EXEC)
		run_exec(cmd, env_st);
	else if (cmd->type == REDIR || cmd->type == HEREDOC
		|| cmd->type == REDIR_IN)
		run_redir(cmd, NULL, 0, env_st);
	else if (cmd->type == PIPE)
		run_pipe(cmd, p, env_st);
	else if (cmd->type == ANDAND || cmd->type == OROR)
		run_bonus(cmd, env_st);
	else
		errors("うーんシェル: fatal error in runcmd: exiting\n");
}

t_cmd	*par_cmd(char *s)
{
	char		*es;
	t_cmd		*cmd;
	static int	brackets = 0;

	es = s + ft_strlen(s);
	cmd = par_line(&s, es, brackets);
	if (cmd == NULL)
		return (NULL);
	next(&s, es, "");
	if (s != es)
	{
		ft_putstr_fd("うーんシェル: syntax error near unexpected token `", 2);
		write(2, s, 1);
		write(2, "'\n", 2);
		free_cmd_tree(cmd);
		return (NULL);
	}
	nulterminate(cmd);
	return (cmd);
}

void	prompt(char *cmdline, struct termios *term, int p_t, t_env_st *env_st)
{
	t_cmd	*cmd;

	while (42)
	{
		env_st->bonus = 1;
		env_st->do_brackets = 1;
		set_signal_handlers();
		close_echoctl(term);
		cmdline = readline(""BC"う"BP"ー"BG"ん"BY"シ"BW"ェ"BR"ル"BB"$ "WHT);
		reset_echoctl(term);
		if (!cmdline)
			prompt_exit();
		if (is_history_worthy(cmdline))
			add_history(cmdline);
		cmd = par_cmd(cmdline);
		if (cmd)
		{
			if (p_t)
				print_tree(cmd, 0);
			run_cmd(cmd, env_st);
			free_cmd_tree(cmd);
		}
		unlink("heredoc.txt");
		free(cmdline);
	}
}

int	main(int argc, char **argv, char **envp)
{
	int				p_t;
	char			*cmdline;
	struct termios	term;
	t_env_st		*env_st;

	p_t = 0;
	if (argc > 2 || (argc == 2 && ft_strcmp(argv[1], "1")))
	{
		printf("Usage: [./minishell] to run in "BB"normal"WHT" mode\n");
		printf("       [./minishell 1] to run in "BC"tree print"WHT" mode\n");
		exit(0);
	}
	if ((argv[1]) && !(ft_strcmp(argv[1], "1")))
		p_t = 1;
	env_st = malloc(sizeof(t_env_st));
	if (!env_st)
		errors("うーんシェル: fatal error mallocing env_st: exiting\n");
	env_st->envp_og = envp;
	g_exit_status = 0;
	create_env_list(env_st);
	env_st->envp_upd = update_envp(env_st);
	tcgetattr(STDIN_FILENO, &term);
	print_uwu();
	prompt(cmdline, &term, p_t, env_st);
	return (0);
}
