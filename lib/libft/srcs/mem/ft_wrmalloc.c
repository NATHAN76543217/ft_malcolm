/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wrmalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlecaill <nlecaill@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/12 18:32:15 by nlecaill          #+#    #+#             */
/*   Updated: 2020/02/17 09:19:22 by nlecaill         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
**	Small wrapper for malloc purpose
**	4 functions
**	-wrgetter : protect list malloc from extern files
**	-wrmalloc : create a malloc, return ptr of the malloc.
**				fail to create malloc : destoy all malloc and return 0
**	-wrfree : free a malloc, return 1.
**				fail to find the element: return 0
**	-wrdestroy : free all malloc created by wrmalloc
*/

static t_list	**wrgetter(void)
{
	static t_list *wrm;

	return (&wrm);
}

void			*wrmalloc(unsigned long size)
{
	char	*buffer;
	t_list	*new_elem;

	if (!(buffer = malloc(size)))
	{
		wrdestroy();
		return (0);
	}
	if (!(new_elem = malloc(sizeof(t_list))))
	{
		free(buffer);
		wrdestroy();
		return (0);
	}
	new_elem->content = buffer;
	new_elem->next = 0;
	ft_lstadd_back(wrgetter(), new_elem);
	return (buffer);
}

int				wrfree(void *ptr)
{
	t_list	*prev;
	t_list	*current;
	t_list	*next;
	t_list	**wrstart;

	prev = 0;
	wrstart = wrgetter();
	current = *wrstart;
	while (current)
	{
		next = current->next;
		if (current->content == ptr)
		{
			free(ptr);
			free(current);
			if (prev)
				prev->next = next;
			else
				*wrstart = next;
			return (1);
		}
		prev = current;
		current = current->next;
	}
	return (0);
}

void			wrdestroy(void)
{
	t_list	*current;
	t_list	*next;
	t_list	**wrstart;

	wrstart = wrgetter();
	current = *wrstart;
	while (current)
	{
		next = current->next;
		free(current->content);
		free(current);
		current = next;
	}
	*wrstart = 0;
}
