/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aotsala <aotsala@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:39:51 by aotsala           #+#    #+#             */
/*   Updated: 2023/09/24 05:40:55 by aotsala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	increment(int *i, int *k)
{
	(*i)++;
	(*k)++;
}

char	*return_quotes(char *output, int output_i, int k)
{
	output[output_i] = '\0';
	if (k % 2)
	{
		n_errors(1, "error: unclosed quotes");
		g_exit_status = 258;
		free(output);
		output = NULL;
	}
	return (output);
}

char	*handle_quotes(char *input, char *output, int i, int output_i)
{
	int	k;

	k = 0;
	while (input[i] != '\0')
	{
		if (input[i] == '"')
		{
			increment(&i, &k);
			while (input[i] != '"' && input[i] != '\0')
				output[output_i++] = input[i++];
			if (input[i] == '"')
				increment(&i, &k);
		}
		else if (input[i] == '\'')
		{
			increment(&i, &k);
			while (input[i] != '\'' && input[i] != '\0')
				output[output_i++] = input[i++];
			if (input[i] == '\'')
				increment(&i, &k);
		}
		else
			output[output_i++] = input[i++];
	}
	return (return_quotes(output, output_i, k));
}

char	**malloc_ret_process_quotes(char **input)
{
	int		i;
	char	**ret;

	i = 0;
	while (input[i])
		i++;
	ret = malloc(sizeof(char **) * (i + 1));
	if (!ret)
		errors("うーんシェル: fatal error mallocing in process quotes: exiting\n");
	return (ret);
}

char	**process_quotes(char **input)
{
	int		i;
	int		k;
	int		output_i;
	char	**output;

	output = malloc_ret_process_quotes(input);
	k = 0;
	while (input[k])
	{
		i = 0;
		output_i = 0;
		output[k] = malloc((sizeof(char) * ft_strlen(input[k]) + 1));
		if (!output[k])
			errors("うーんシェル: fatal error mallocing process quotes: exiting\n");
		output[k] = handle_quotes(input[k], output[k], i, output_i);
		if (output[k++] == NULL)
		{
			free_double_pointer(output);
			free_double_pointer(input);
			return (NULL);
		}
	}
	output[k] = NULL;
	free_double_pointer(input);
	return (output);
}
