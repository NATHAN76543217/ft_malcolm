/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgascon <dgascon@student.le-101.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/13 05:16:32 by dgascon           #+#    #+#             */
/*   Updated: 2020/02/17 09:19:22 by dgascon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** The ft_strlcpy() function copy strings and copies up to size - 1 characters
** from the NUL-terminated string src to dst, NUL-terminating the result.
*/

size_t		ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t i;
	size_t size_src;

	i = 0;
	if (src == 0)
		return (0);
	size_src = ft_strlen(src);
	if (size <= 0)
		return (size_src);
	while (i < size - 1 && *(src + i))
	{
		*(dst + i) = *(src + i);
		i++;
	}
	*(dst + i) = '\0';
	return (size_src);
}
