/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgascon <dgascon@student.le-101.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/12 02:24:26 by dgascon           #+#    #+#             */
/*   Updated: 2020/02/17 09:19:22 by dgascon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** The ft_memcpy() function copies n bytes from memory area src to memory area
** dst.
*/

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	char		*pdst;
	const char	*psrc;

	if (!dst && !src)
		return (NULL);
	pdst = dst;
	psrc = src;
	while (n-- > 0)
		*(pdst + n) = *(psrc + n);
	return (dst);
}
