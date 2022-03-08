/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgascon <dgascon@student.le-101.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/21 20:15:13 by dgascon           #+#    #+#             */
/*   Updated: 2020/03/09 18:33:15 by nlecaill         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	trimcmp(char const c1, char const *s2)
{
	while (*s2)
		if (c1 == *s2++)
			return (1);
	return (0);
}

char		*ft_strtrim(char const *s1, char const *set)
{
	int		max;
	int		min;
	int		i;
	char	*ptr;

	if (!s1)
		return (NULL);
	i = 0;
	min = 0;
	max = ft_strlen(s1);
	while (trimcmp(s1[min], set))
		min++;
	while (trimcmp(s1[max - 1], set))
		max--;
	(max < min) ? max = min + 1 : 0;
	if (!(ptr = wrmalloc((max - min + 1) * sizeof(char))))
		return (NULL);
	while (s1[i] && i < max - min)
	{
		ptr[i] = s1[i + min];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}
