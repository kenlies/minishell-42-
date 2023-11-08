/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_extra.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aotsala <aotsala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:39:51 by aotsala           #+#    #+#             */
/*   Updated: 2023/09/27 13:30:27 by aotsala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_curr_dir(void)
{
	char	buf[PATH_LEN];
	char	*dir;

	getcwd(buf, PATH_LEN);
	dir = ft_strdup(buf);
	if (!dir)
		errors("うーんシェル: fatal error mallocing: exiting\n");
	return (dir);
}

char	*get_path_val(char *var, t_env_st *env_st)
{
	char	*path;
	t_env	*env;

	env = locate_env_var(var, env_st);
	if (!env)
		return (NULL);
	path = env->val;
	return (path);
}

char	*add_eq(char *value)
{
	char	*new;
	int		i;

	i = 0;
	new = malloc(sizeof(char) * ft_strlen(value) + 2);
	new[0] = '=';
	while (value[i])
	{
		new[i + 1] = value[i];
		i++;
	}
	new[i + 1] = '\0';
	return (new);
}
