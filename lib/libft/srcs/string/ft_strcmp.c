/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgascon <dgascon@student.le-101.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/27 15:50:52 by dgascon           #+#    #+#             */
/*   Updated: 2020/02/17 09:19:22 by dgascon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strcmp(char *s1, char *s2)
{
	unsigned char	*st1;
	unsigned char	*st2;
	int				i;
	int				i2;

	i = 0;
	i2 = 0;
	st1 = (unsigned char *)s1;
	st2 = (unsigned char *)s2;
	while (s1[i] && s2[i2])
	{
		if (s1[i] != s2[i2])
		{
			return (st1[i] - st2[i2]);
		}
		i2++;
		i++;
	}
	return (st1[i] - st2[i2]);
}
