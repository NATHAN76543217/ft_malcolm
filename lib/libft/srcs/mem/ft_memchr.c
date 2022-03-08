/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgascon <dgascon@student.le-101.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/10 15:16:19 by dgascon           #+#    #+#             */
/*   Updated: 2020/02/17 09:19:22 by dgascon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** The ft_memchr() function locates the first occurrence of c in string s.
*/

void	*ft_memchr(const void *str, int c, size_t n)
{
	char const	*pstr;
	size_t		i;

	pstr = str;
	i = 0;
	while (i < n)
	{
		if (*(pstr + i) == c)
			return ((void *)&(*(pstr + i)));
		i++;
	}
	return (0);
}
