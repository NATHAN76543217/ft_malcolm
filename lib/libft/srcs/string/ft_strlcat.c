/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgascon <dgascon@student.le-101.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/13 08:18:49 by dgascon           #+#    #+#             */
/*   Updated: 2020/02/17 09:19:22 by dgascon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** The ft_strlcat() function concatenate strings and appends the NUL-terminated
** string src to the end of dst.
*/

size_t		ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	dst_size;
	size_t	src_size;

	i = 0;
	dst_size = 0;
	src_size = 0;
	while (src[src_size])
		src_size++;
	while (dst_size < size && dst[dst_size])
		dst_size++;
	if (size <= dst_size)
		return (src_size + size);
	else
		src_size = src_size + dst_size;
	while (src[i] != '\0' && dst_size < (size - 1))
	{
		dst[dst_size] = src[i];
		i++;
		dst_size++;
		dst[dst_size] = '\0';
	}
	return (src_size);
}
