/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aotsala <aotsala@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:39:51 by aotsala           #+#    #+#             */
/*   Updated: 2023/09/24 07:21:49 by aotsala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	tokenizer_filename_cmd(char **ps, char *es, int ret, const char *whitespace)
{
	const char	symbol[] = "<|>()";

	ret = 'a';
	while (*ps < es && !ft_strchr(whitespace, **ps) && !ft_strchr(symbol, **ps))
	{
		if (**ps == '"')
		{
			(*ps)++;
			while (**ps != '"' && *ps < es)
				(*ps)++;
			if (**ps == '"')
				(*ps)++;
		}
		else if (**ps == '\'')
		{
			(*ps)++;
			while (**ps != '\'' && *ps < es)
				(*ps)++;
			if (**ps == '\'')
				(*ps)++;
		}
		else
			(*ps)++;
	}
	return (ret);
}

int	tokenizer_andand(char **ps, int ret)
{
	ret = '@';
	(*ps) += 2;
	return (ret);
}

int	tokenizer_oror(char **ps, int ret)
{
	(*ps)++;
	if (**ps == '|')
	{
		ret = '!';
		(*ps)++;
	}
	return (ret);
}

int	tokenizer_redirs(char **ps, int ret)
{
	if (**ps == '>')
	{
		(*ps)++;
		if (**ps == '>')
		{
			ret = '+';
			(*ps)++;
		}
	}
	else if (**ps == '<')
	{
		(*ps)++;
		if (**ps == '<')
		{
			ret = '-';
			(*ps)++;
		}
	}
	return (ret);
}

int	tokenizer(char **ps, char *es, char **q, char **eq)
{
	const char	whitespace[] = " \t\r\n\v";
	int			ret;

	while (*ps < es && ft_strchr(whitespace, **ps))
		(*ps)++;
	if (q)
		*q = *ps;
	ret = **ps;
	if (**ps == '\0')
		return (ret);
	else if (**ps == '(' || **ps == ')')
		(*ps)++;
	else if ((**ps == '>') || (**ps == '<'))
		ret = tokenizer_redirs(ps, ret);
	else if (**ps == '|' )
		ret = tokenizer_oror(ps, ret);
	else if (**ps == '&' && *(*ps + 1) == '&')
		ret = tokenizer_andand(ps, ret);
	else
		ret = tokenizer_filename_cmd(ps, es, ret, whitespace);
	if (eq)
		*eq = *ps;
	while (*ps < es && ft_strchr(whitespace, **ps))
		(*ps)++;
	return (ret);
}
