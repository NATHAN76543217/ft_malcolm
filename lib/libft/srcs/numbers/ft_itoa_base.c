/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgascon <dgascon@student.le-101.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/11 13:32:58 by nlecaill          #+#    #+#             */
/*   Updated: 2020/02/17 09:21:18 by dgascon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		ft_length(int n, int base)
{
	int i;

	i = 0;
	if (n < 0)
		i++;
	if (n == 0)
		return (1);
	while (n != 0)
	{
		n = n / base;
		i++;
	}
	return (i);
}

char			*ft_itoa_base(int n, char *charset)
{
	char	*str;
	int		i;
	int		sign;
	int		base;

	base = ft_strlen(charset);
	sign = 1;
	i = ft_length(n, base);
	if (!(str = ft_calloc(i + 1, sizeof(char))))
		return (NULL);
	if (n < 0)
	{
		str[0] = '-';
		sign = -1;
	}
	i--;
	if (n == 0)
		str[i] = '0';
	while (n != 0)
	{
		str[i] = charset[(n % base) * sign];
		n = n / base;
		i--;
	}
	return (str);
}
