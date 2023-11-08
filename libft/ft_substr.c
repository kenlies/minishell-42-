/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aotsala <aotsala@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 22:45:42 by aotsala           #+#    #+#             */
/*   Updated: 2023/09/24 04:22:05 by aotsala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, int len)
{
	char	*sub;
	int		i;

	if (len >= ft_strlen(s))
		len = ft_strlen(s);
	if (!s)
		return (NULL);
	i = 0;
	sub = (char *)(malloc(sizeof(char) * len + 1));
	if (!sub)
		errors("うーんシェル: fatal error mallocing in substr: exiting\n");
	if ((int)start >= ft_strlen(s))
	{
		sub[i] = '\0';
		return (sub);
	}
	while (i < len)
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}
