/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aotsala <aotsala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:39:51 by aotsala           #+#    #+#             */
/*   Updated: 2023/09/27 13:22:50 by aotsala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**malloc_ret_new_cmdline(int i)
{
	char	**ret;

	ret = malloc(sizeof(char *) * (i + 1));
	if (!ret)
		errors("うーんシェル: fatal error mallocing in wildcards: exiting\n");
	return (ret);
}

void	handle_first_arg_helper(char **cmdline, char **cmdline_new,
			struct dirent *ent)
{
	int	i;

	if (is_matching(ent->d_name, cmdline[0]))
	{
		i = 1;
		free(cmdline_new[0]);
		cmdline_new[0] = ft_strdup(ent->d_name);
		while (cmdline[i])
		{
			free(cmdline_new[i]);
			cmdline_new[i] = ft_strdup(cmdline[i]);
			i++;
		}
		cmdline_new[i] = NULL;
	}
}

char	**handle_first_arg(char **cmdline, char **cmdline_new)
{
	DIR				*dir;
	struct dirent	*ent;

	if (ft_strchr(cmdline[0], '*'))
	{
		dir = opendir(".");
		ent = readdir(dir);
		while (ent)
		{
			handle_first_arg_helper(cmdline, cmdline_new, ent);
			ent = readdir(dir);
		}
		closedir(dir);
		return (cmdline_new);
	}
	return (cmdline);
}
