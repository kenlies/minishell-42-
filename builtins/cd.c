/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aotsala <aotsala@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:39:51 by aotsala           #+#    #+#             */
/*   Updated: 2023/09/24 04:11:57 by aotsala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	dir_handler(char *path)
{
	DIR		*dir;

	dir = opendir(path);
	if (dir == NULL)
	{
		if (errno == ENOTDIR)
		{
			n_errors(3, "cd: ", path, ": Not a directory");
			return (1);
		}
		else if (errno == EACCES)
		{
			n_errors(3, "cd: ", path, ": Permission denied");
			return (1);
		}
		else if (errno == ENOENT)
		{
			n_errors(3, "cd: ", path, ": No such file or directory");
			return (1);
		}
	}
	else
		closedir(dir);
	return (0);
}

void	update_env_list_cd(char *variable, char *value, t_env_st *env_st)
{
	t_env	*remove;
	char	*var;
	char	*val;

	var = ft_strdup(variable);
	val = add_eq(value);
	if (!var || !val)
		errors("うーんシェル: fatal error mallocing: exiting\n");
	remove = locate_env_var(var, env_st);
	if (remove)
		remove_env_var(remove, env_st);
	if (!add_to_env_list(var, val, env_st))
	{
		free(var);
		free(val);
	}
}

int	c_dir(char *path, t_env_st *env_st)
{
	char	*path_old;
	char	*path_curr;

	if (dir_handler(path) == 0)
	{
		path_old = get_curr_dir();
		if (chdir(path) == 0)
		{
			path_curr = get_curr_dir();
			update_env_list_cd("OLDPWD", path_old, env_st);
			update_env_list_cd("PWD", path_curr, env_st);
			free(path_old);
			free(path_curr);
			return (1);
		}
		else
			n_errors(2, "cd: dir error: ", path);
		free(path_old);
	}
	return (0);
}

int	cd(char **args, t_env_st *env_st)
{
	char	*home;

	args++;
	while (args && *args && ft_strcmp(*args, "\0") == 0)
		args++;
	if (!(*args) || !(**args))
	{
		home = get_path_val("HOME", env_st);
		if (!home)
		{
			n_errors(2, "cd: ", "HOME not set");
			return (1);
		}
		home++;
		if (c_dir(home, env_st))
			return (0);
	}
	else
	{
		if (c_dir(*args, env_st))
			return (0);
	}
	return (1);
}
