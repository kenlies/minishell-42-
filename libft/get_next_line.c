/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aotsala <aotsala@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 16:45:58 by aotsala           #+#    #+#             */
/*   Updated: 2023/04/29 02:11:14 by aotsala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_merge(char *stash, char *buf)
{
	char	*store;
	int		i;
	int		j;

	if (!stash || !buf)
		return (NULL);
	store = (char *)malloc(ft_strlen(stash) + ft_strlen(buf) + 1);
	if (!store)
		return (NULL);
	i = 0;
	while (stash[i] != '\0')
	{
		store[i] = stash[i];
		i++;
	}
	j = 0;
	while (buf[j] != '\0')
		store[i++] = buf[j++];
	store[ft_strlen(stash) + ft_strlen(buf)] = '\0';
	free(stash);
	return (store);
}

char	*make_new_stash(char *stash)
{
	int		i;
	int		j;
	char	*newstash;

	i = 0;
	while (stash[i] != '\n' && stash[i] != '\0')
		i++;
	if (stash[i] == '\0')
	{
		free(stash);
		return (NULL);
	}
	newstash = (char *)malloc(sizeof(char) * (ft_strlen(stash) - i));
	if (!newstash)
		return (NULL);
	j = 0;
	while (stash[i++] != '\0')
		newstash[j++] = stash[i];
	free(stash);
	return (newstash);
}

char	*get_line(char *stash)
{
	int		i;
	int		len;
	char	*line;

	len = 0;
	if (stash[0] == '\0')
		return (NULL);
	while (stash[len] != '\0' && stash[len] != '\n')
		len++;
	if (stash[len] == '\n')
		len++;
	line = (char *)malloc(sizeof(char) * len + 1);
	i = 0;
	while (i < len)
	{
		line[i] = stash[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

char	*read_to_stash(int fd, char *stash)
{
	char	*buf;
	int		bytes;

	bytes = 1;
	buf = (char *)malloc(sizeof(char) * BUFFER_SIZE + 1);
	if (!buf)
		return (NULL);
	while (!ft_strchr(stash, '\n') && bytes != 0)
	{
		bytes = read(fd, buf, BUFFER_SIZE);
		if (bytes < 0)
		{
			free(buf);
			return (NULL);
		}
		if (!stash)
		{
			stash = (char *)malloc(sizeof(char ) * 1);
			stash[0] = '\0';
		}
		buf[bytes] = '\0';
		stash = ft_merge(stash, buf);
	}
	free(buf);
	return (stash);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*stash[5000];

	if (fd < 0 || fd > 4095 || BUFFER_SIZE <= 0)
		return (NULL);
	stash[fd] = read_to_stash(fd, stash[fd]);
	if (!stash[fd])
		return (NULL);
	line = get_line(stash[fd]);
	stash[fd] = make_new_stash(stash[fd]);
	return (line);
}
