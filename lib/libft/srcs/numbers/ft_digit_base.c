/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_digit_base.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlecaill <nlecaill@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/11 14:32:47 by nlecaill          #+#    #+#             */
/*   Updated: 2020/02/17 09:19:22 by nlecaill         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_digit_base(long num, char *base)
{
	int i;
	int sbase;

	sbase = ft_strlen(base);
	i = 0;
	if (num < 0)
		i++;
	if (num == 0)
		return (1);
	while (num != 0)
	{
		num = num / sbase;
		i++;
	}
	return (i);
}
