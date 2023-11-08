/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_bonus_pipe.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aotsala <aotsala@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:39:51 by aotsala           #+#    #+#             */
/*   Updated: 2023/09/25 00:32:56 by aotsala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	run_bonus_fuck_norm_two(t_bonuscmd *bcmd,
			t_bonuscmd *right, t_env_st *env_st)
{
	if (g_exit_status == 0)
		env_st->bonus = 0;
	if (bcmd->right->type == OROR || bcmd->right->type == ANDAND)
	{
		right = (t_bonuscmd *)bcmd->right;
		if (bcmd->brackets < right->brackets && env_st->bonus == 0)
			env_st->do_brackets = 0;
		else if (bcmd->brackets < right->brackets && env_st->bonus == 1)
			env_st->do_brackets = 1;
	}
}

void	run_bonus_fuck_norm_one(t_bonuscmd *bcmd,
			t_bonuscmd *right, t_env_st *env_st)
{
	if (g_exit_status != 0)
		env_st->bonus = 0;
	if (bcmd->right->type == OROR || bcmd->right->type == ANDAND)
	{
		right = (t_bonuscmd *)bcmd->right;
		if (bcmd->brackets < right->brackets && env_st->bonus == 0)
			env_st->do_brackets = 0;
		else if (bcmd->brackets < right->brackets && env_st->bonus == 1)
			env_st->do_brackets = 1;
	}
}

void	run_bonus(t_cmd *cmd, t_env_st *env_st)
{
	t_bonuscmd	*bcmd;
	t_bonuscmd	*right;

	bcmd = (t_bonuscmd *)cmd;
	run_cmd(bcmd->left, env_st);
	if (bcmd->type == ANDAND)
	{
		run_bonus_fuck_norm_one(bcmd, right, env_st);
		run_cmd(bcmd->right, env_st);
	}
	else if (bcmd->type == OROR)
	{
		run_bonus_fuck_norm_two(bcmd, right, env_st);
		run_cmd(bcmd->right, env_st);
	}
}

void	run_pipe_do_forks(t_pipecmd *pcmd, int *p, t_env_st *env_st)
{
	pid_t		fork1;
	pid_t		fork2;

	fork1 = forky();
	if (fork1 == 0)
	{
		dup2(p[1], 1);
		close(p[0]);
		close(p[1]);
		run_cmd(pcmd->left, env_st);
		exit(g_exit_status);
	}
	fork2 = forky();
	if (fork2 == 0)
	{
		dup2(p[0], 0);
		close(p[0]);
		close(p[1]);
		run_cmd(pcmd->right, env_st);
		exit(g_exit_status);
	}
	close(p[0]);
	close(p[1]);
	wait_child(fork1);
	wait_child(fork2);
}

void	run_pipe(t_cmd *cmd, int *p, t_env_st *env_st)
{
	t_pipecmd	*pcmd;

	pcmd = (t_pipecmd *)cmd;
	if (pipe(p) < 0)
		errors("うーんシェル: fatal error in pipe: exiting\n");
	run_pipe_do_forks(pcmd, p, env_st);
}
