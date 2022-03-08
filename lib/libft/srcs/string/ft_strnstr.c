/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgascon <dgascon@student.le-101.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/13 11:06:27 by dgascon           #+#    #+#             */
/*   Updated: 2020/02/17 09:19:22 by dgascon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** The ft_strnstr() function locates the first occurrence of the
** null-terminated string needle in the string haystack, where not more than
** len characters are searched.
*/

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	hay_i;
	size_t	need_i;

	hay_i = 0;
	need_i = 0;
	if (ft_strlen(needle) <= 0)
		return ((char *)haystack);
	while (*(haystack + hay_i))
	{
		need_i = 0;
		while (*(needle + need_i) == *(haystack + hay_i + need_i))
		{
			if (hay_i + need_i >= len)
				return (NULL);
			if (*(needle + need_i + 1) == '\0')
				return (&(*((char *)haystack + hay_i)));
			need_i++;
		}
		hay_i++;
	}
	return (NULL);
}
