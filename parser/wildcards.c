/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aotsala <aotsala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:39:51 by aotsala           #+#    #+#             */
/*   Updated: 2023/09/27 13:11:49 by aotsala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_matching(char *ent, char *pat)
{
	if (*pat == '*')
	{
		if (*ent)
			return (is_matching(ent + 1, pat) || is_matching(ent, pat + 1));
		return (is_matching(ent, pat + 1));
	}
	if (*ent == *pat)
	{
		if (*ent)
			return (is_matching(ent + 1, pat + 1));
		return (1);
	}
	return (0);
}

char	**make_new_cmdline(char **cmdline, const char *filename)
{
	int		i;
	int		j;
	int		flag;
	char	**cmdline_new;

	flag = 0;
	i = 0;
	while (cmdline[i])
		if (ft_strchr(cmdline[i++], '*'))
			flag = 1;
	if (!flag)
		i++;
	cmdline_new = malloc_ret_new_cmdline(i);
	i = 0;
	j = 0;
	while (cmdline[i])
	{
		if (!ft_strchr(cmdline[i], '*'))
			cmdline_new[j++] = ft_strdup(cmdline[i]);
		i++;
	}
	cmdline_new[j++] = ft_strdup(filename);
	cmdline_new[j] = NULL;
	free_double_pointer(cmdline);
	return (cmdline_new);
}

char	**find_matches(char **cmdline, char **cmdline_new)
{
	int				i;
	DIR				*dir;
	struct dirent	*ent;
	char			**cmdline_tmp;

	cmdline_tmp = handle_first_arg(cmdline, cmdline_new);
	i = 0;
	while (cmdline_tmp[i])
	{
		if (ft_strchr(cmdline_tmp[i], '*'))
		{
			dir = opendir(".");
			ent = readdir(dir);
			while (ent)
			{
				if (is_matching(ent->d_name, cmdline_tmp[i]))
					cmdline_new = make_new_cmdline(cmdline_new, ent->d_name);
				ent = readdir(dir);
			}
			closedir(dir);
		}
		i++;
	}
	free_double_pointer(cmdline);
	return (cmdline_new);
}

char	**handle_wildcards(char	**cmdline)
{
	int				i;
	char			**cmdline_new;

	i = 0;
	while (cmdline[i] != NULL)
		i++;
	cmdline_new = malloc(sizeof(char **) * (i + 1));
	if (!cmdline_new)
		errors("うーんシェル: fatal error mallocing handle_wildcards: exiting\n");
	i = 0;
	while (cmdline[i])
	{
		cmdline_new[i] = ft_strdup(cmdline[i]);
		i++;
	}
	cmdline_new[i] = NULL;
	cmdline_new = find_matches(cmdline, cmdline_new);
	return (cmdline_new);
}
