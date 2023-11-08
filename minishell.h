/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aotsala <aotsala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:39:51 by aotsala           #+#    #+#             */
/*   Updated: 2023/09/27 13:22:27 by aotsala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <signal.h>
# include <unistd.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <termios.h>
# include <dirent.h>
# include <errno.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# define PATH_LEN 4096
# define MAXARGS 20
# define PIPE 1
# define REDIR 2
# define REDIR_IN 3
# define EXEC 4
# define HEREDOC 5
# define ANDAND 6
# define OROR 7
# define BP "\001\e[1;95m\002"
# define BB "\001\e[1;30m\002"
# define BR "\001\e[1;31m\002"
# define BG "\001\e[1;92m\002"
# define BY "\001\e[1;33m\002"
# define BBL "\001\e[1;34m\002"
# define BM "\001\e[1;35m\002"
# define BC "\001\e[1;36m\002"
# define BW "\001\e[1;37m\002"
# define WHT "\001\e[0;m\002"
# define US "\x1B[4m"
# define UR "\x1B[0m"

typedef struct s_incs
{
	int	i;
	int	k;
}				t_incs;

int			g_exit_status;

typedef struct s_env
{
	char			*var;
	char			*val;
	struct s_env	*next;
}					t_env;

typedef struct s_env_st
{
	char			**envp_og;
	char			**envp_upd;
	struct s_env	*envlist;
	int				bonus;
	int				do_brackets;
}					t_env_st;

typedef struct s_cmd
{
	int	type;
}				t_cmd;

typedef struct s_execcmd
{
	int		type;
	char	*argv[MAXARGS];
	char	*eargv[MAXARGS];
}				t_execcmd;

typedef struct s_redircmd
{
	int		type;
	t_cmd	*cmd;
	char	*file;
	char	*efile;
	int		mode;
	int		fd;
}				t_redircmd;

typedef struct s_pipecmd
{
	int		type;
	t_cmd	*left;
	t_cmd	*right;
}				t_pipecmd;

typedef struct s_bonuscmd
{
	int		type;
	t_cmd	*left;
	t_cmd	*right;
	int		brackets;
}				t_bonuscmd;

// main.c
void		run_cmd(t_cmd *cmd, t_env_st *env_st);
t_cmd		*par_cmd(char *s);
void		prompt(char *cmdline, struct termios *term,
				int p_t, t_env_st *env_st);
int			main(int argc, char **argv, char **envp);
void		prompt_exit(void);

// utils.c
long long	ft_atoll(char *str);
int			check_long_long(char *str);
int			is_history_worthy(char *cmdline);
int			check_for_oror(char **ps);
char		**stupid_ahh_function_for_norm(char **ret, char **argv);

// free.c
void		free_cmd_tree(t_cmd *cmd);
void		free_double_pointer(char **args);
void		free_and_update_env_list_array(t_env_st *env_st);

// errors.c
void		find_dir_error(char *command, struct stat st);
void		find_correct_error_msg(char	*command);
void		errors(char *msg);
void		n_errors(int n, ...);
void		*parse_error(t_cmd *cmd, char *msg);

// quotes.c
void		increment(int *i, int *k);
char		**process_quotes(char **input);
char		*return_quotes(char *output, int output_i, int k);
char		*handle_quotes(char *input, char *output, int i, int output_i);
char		**malloc_ret_process_quotes(char **input);

// constructors.c
t_cmd		*exec_cmd(void);
t_cmd		*redir_cmd(t_cmd *subcmd, char *file, char *efile, int mode);
t_cmd		*pipe_cmd(t_cmd *left, t_cmd *right);
t_cmd		*bonus_cmd(t_cmd *left, t_cmd *right, int type, int brackets);

// run_redir_exec.c
char		*get_path(char *cmd_str, t_env_st *env_st);
void		run_exec_handle_sys(char **q_argv, t_env_st *env_st);
void		run_exec(t_cmd *cmd, t_env_st *env_st);
void		run_redir_continuation(t_redircmd *rcmd,
				t_redircmd *node, t_env_st *env_st);
void		run_redir(t_cmd *cmd, t_redircmd *carry,
				int flag, t_env_st *env_st);

// run_utils.c
int			forky(void);
void		wait_child(pid_t pid);
void		ambiguous(t_redircmd *rcmd, int old_fd);
int			create_dummy_file(t_redircmd *dummy);
char		*build_cmd_str(char *cmd_str, t_env_st *env_st);

// run_builtin.c
int			is_builtin(char **args);
int			run_builtin(char **args, t_env_st *env_st);

// run_bonus_pipe.c
void		run_bonus(t_cmd *cmd, t_env_st *env_st);
void		run_bonus_fuck_norm_one(t_bonuscmd *bcmd,
				t_bonuscmd *right, t_env_st *env_st);
void		run_bonus_fuck_norm_two(t_bonuscmd *bcmd,
				t_bonuscmd *right, t_env_st *env_st);
void		run_pipe_do_forks(t_pipecmd *pcmd, int *p, t_env_st *env_st);
void		run_pipe(t_cmd *cmd, int *p, t_env_st *env_st);

// parse.c
t_cmd		*par_exec(char **ps, char *es, int brackets);
t_cmd		*par_line(char **ps, char *es, int brackets);
t_cmd		*par_pipe(char **ps, char *es, int brackets);
t_cmd		*par_redirs(t_cmd *cmd, char **ps, char *es);
t_cmd		*par_bonus(t_cmd *cmd, char **ps, char *es, int brackets);

// parse_heredoc.c
void		free_and_close(char *delimeter, int fd);
char		*remove_quotes_from_delim(char *delimeter);
char		*get_delim(char *beg_delim, char *end_delim);
void		heredoc_buf(char **buf);
void		do_heredoc(char *beg_delim, char *end_delim);

// parse_token.c
int			tokenizer_filename_cmd(char **ps, char *es,
				int ret, const char *whitespace);
int			tokenizer_andand(char **ps, int ret);
int			tokenizer_oror(char **ps, int ret);
int			tokenizer_redirs(char **ps, int ret);
int			tokenizer(char **ps, char *es, char **q, char **eq);

// parse_utils.c
t_cmd		*par_exec_loop(char **ps, char *es, t_execcmd *cmd, t_cmd *ret);
t_cmd		*par_brackets(char **ps, char *es, int brackets);
int			next_bonus(char **ps, char *es, char *toks1, char *toks2);
int			next(char **ps, char *es, char *toks);
void		nulterminate(t_cmd	*cmd);

// wildcards.c
int			is_matching(char *entry, char *pattern);
char		**make_new_cmdline(char **cmdline, const char *filename);
char		**find_matches(char **cmdline, char **cmdline_new);
char		**handle_wildcards(char	**cmdline);

// wildcard_utils.c
char		**malloc_ret_new_cmdline(int i);
char		**handle_first_arg(char **cmdline, char **cmdline_new);
void		handle_first_arg_helper(char **cmdline, char **cmdline_new,
				struct dirent *ent);

// var.c
void		expand_dq(t_incs *incs, char **ret, char **str, t_env_st *env_st);
void		expand_sq(t_incs *incs, char **ret, char **str);
void		expand_dol(t_incs *incs, char **ret, char **str, t_env_st *env_st);
char		*expand(char *str, t_env_st *env_st);
char		**process_vars(char **argv, t_env_st *env_st);

// var_utils.c
int			expand_dq_dol_q_mark(int *i, int *k, char **ret, char **str);
void		copy_to_ret(char **ret, char *val, int *index);
char		*create_exp_var(char *dollar_p);
int			is_equal(char *argv);
char		**is_equal_quote_count(char **argv);

// var_len.c
int			find_exp_val_len_ret_val(char *str, int index, char **val, 
				t_env_st *env_st);
int			find_exit_len(int exit_status);
int			handle_dq_len(int *i, int *len, char **str, t_env_st *env_st);
int			handle_dol_len(int *i, char **str, t_env_st *env_st);
int			get_var_len(char *str, t_env_st *env_st);

// env_list.c
void		create_env_list(t_env_st *env_st);
char		**dublicate_env(t_env_st *env_st);
char		**update_envp(t_env_st *env_st);

// env_utils.c
void		remove_env_var(t_env *remove, t_env_st *env_st);
int			valid_var_name(char *arg);
t_env		*locate_env_var(char *var, t_env_st *env_st);
char		*get_var_val(char *var_str, int mode);
char		*ft_getenv(char *var, t_env_st *env_st);

// env_ft.c
t_env		*ft_lstnew_env(char *var, char *val);
int			ft_lstsize_env(t_env *lst);
void		ft_lstadd_back_env(t_env **head, t_env *new);

// sig_set.c
void		set_signal_child(void);
void		set_signal_handlers(void);
void		set_signal_handlers_heredoc(void);

// sig_utils.c
void		handler(int signal);
void		handler_heredoc(int signal);
void		handler_child(int signal);
void		close_echoctl(struct termios *t);
void		reset_echoctl(struct termios *t);

//********//********//builtins//********//********//

// env.c
void		print_linked_list(t_env_st *env_st);
int			env(char **args, t_env_st *env_st);

// unset.c
int			unset(char **args, t_env_st *env_st);

// export.c
int			add_to_env_list(char *var, char *val, t_env_st *env_st);
int			modify_env_list(t_env *replace, char *var, char *val,
				t_env_st *env_st);
int			export(char **args, t_env_st *env_st);

// exit.c
int			is_valid_exit_num(char *arg);
int			exit_mini(char **args);

// pwd.c
int			pwd(void);

// echo.c
int			echo_do(char ***args);
int			echo(char **args);

// cd.c
void		update_env_list_cd(char *variable, char *value, t_env_st *env_st);
int			dir_handler(char *path);
int			c_dir(char *path, t_env_st *env_st);
int			cd(char **args, t_env_st *env_st);

// cd_extra.c
char		*get_curr_dir(void);
char		*get_path_val(char *var, t_env_st *env_st);
char		*add_eq(char *value);

//********//********//rizzslay//********//********//

void		print_uwu(void);
void		print_top_1(void);
void		print_top_11(void);
void		print_top_2(void);
void		print_top_22(void);
void		print_top_3(void);
void		print_top_33(void);
void		print_top_4(void);
void		print_top_44(void);
void		print_top_5(void);
void		print_top_55(void);
void		print_top_6(void);
void		print_top_66(void);
void		print_rizzer1(void);
void		print_rizzer2(void);
void		print_rizzer3(void);
void		print_rizzer4(void);
void		print_rizzer5(void);

//********//********//print_tree//********//********//

void		print_exec(t_cmd *cmd);
void		print_redir(t_cmd *cmd, int ind);
void		print_pipe(t_cmd *cmd);
void		print_tabs(int ind);
void		print_tree(t_cmd *cmd, int ind);

#endif
