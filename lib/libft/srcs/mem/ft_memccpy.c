/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgascon <dgascon@student.le-101.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/09 20:13:02 by dgascon           #+#    #+#             */
/*   Updated: 2020/02/17 09:19:22 by dgascon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** The memccpy() function copies bytes from string src to string dst.
** If the character c (as converted to an unsigned char) occurs in the string
** src, the copy stops and a pointer to the byte after the copy of c in the
** string dst is returned.
** Otherwise, n bytes are copied, and a NULL pointer is returned.
** The source and destination strings should not overlap, as the behavior is
** undefined.
*/

void	*ft_memccpy(void *dst, void const *src, int c, size_t n)
{
	unsigned char const	*psrc;
	char				*pdst;
	size_t				i;

	if (!dst && !src)
		return (0);
	i = 0;
	psrc = src;
	pdst = dst;
	while (i < n)
	{
		*(pdst + i) = *(psrc + i);
		if (*(psrc + i) == (unsigned char)c)
			return (&(*(pdst + i + 1)));
		i++;
	}
	return (NULL);
}
