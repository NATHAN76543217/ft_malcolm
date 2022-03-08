/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libftlst.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlecaill <nlecaill@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/20 18:32:02 by dgascon           #+#    #+#             */
/*   Updated: 2020/02/17 09:18:12 by dgascon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFTLST_H
# define LIBFTLST_H

typedef struct	s_list
{
	void			*content;
	struct s_list	*next;
}				t_list;
t_list			*ft_lstnew(void *content);
void			ft_lstadd_front(t_list **alst, t_list *enew);
int				ft_lstsize(t_list *lst);
t_list			*ft_lstlast(t_list *lst);
void			ft_lstadd_back(t_list **alst, t_list *enew);
void			ft_lstdelone(t_list *lst, void (*del)(void *));
void			ft_lstclear(t_list **lst, void (*del)(void *));
void			ft_lstiter(t_list *lst, void (*f)(void *));
t_list			*ft_lstmap(t_list *lst, void *(*f)(void *), void
(*del)(void *));
#endif
