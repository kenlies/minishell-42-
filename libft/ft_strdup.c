/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aotsala <aotsala@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 22:04:39 by aotsala           #+#    #+#             */
/*   Updated: 2023/09/24 04:22:34 by aotsala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	size_t	size;
	char	*p1;
	void	*p2;

	size = ft_strlen(s);
	p1 = (char *)s;
	p2 = malloc((size + 1) * sizeof(char));
	if (p2)
		ft_memcpy(p2, s, size + 1);
	else
		errors("うーんシェル: fatal error mallocing in strdub: exiting\n");
	return (p2);
}
