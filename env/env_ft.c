/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_ft.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aotsala <aotsala@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:39:51 by aotsala           #+#    #+#             */
/*   Updated: 2023/08/18 13:13:57 by aotsala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_lstsize_env(t_env *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}

void	ft_lstadd_back_env(t_env **head, t_env *new)
{
	t_env	*current;

	current = *head;
	if (head)
	{
		if (*head == NULL)
			*head = new;
		else
		{
			while (current->next)
				current = current->next;
			current->next = new;
		}
	}
}

t_env	*ft_lstnew_env(char *var, char *val)
{
	t_env	*node;

	node = (t_env *)malloc(sizeof(*node));
	if (!node)
		return (NULL);
	node->var = var;
	node->val = val;
	node->next = NULL;
	return (node);
}
