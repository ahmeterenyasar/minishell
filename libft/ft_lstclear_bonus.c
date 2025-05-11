/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 11:34:56 by ayasar            #+#    #+#             */
/*   Updated: 2024/10/27 14:47:38 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*head;
	t_list	*tmp;

	if (!lst || !del || !*lst)
		return ;
	head = *lst;
	while (head)
	{
		tmp = head->next;
		(*del)(head->content);
		free(head);
		head = tmp;
	}
	*lst = NULL;
}
