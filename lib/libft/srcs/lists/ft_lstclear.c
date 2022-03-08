/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgascon <dgascon@student.le-101.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/29 13:12:24 by dgascon           #+#    #+#             */
/*   Updated: 2020/02/17 09:19:22 by dgascon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	lstclear(t_list *lst, void (*del)(void *))
{
	if (lst->next)
		lstclear(lst->next, del);
	ft_lstdelone(lst, del);
}

void		ft_lstclear(t_list **lst, void (*del)(void *))
{
	if (!lst)
		return ;
	lstclear(*lst, del);
	*lst = 0;
}
