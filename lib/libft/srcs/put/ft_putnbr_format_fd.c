/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_format_fd.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgascon <dgascon@student.le-101.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/18 01:42:15 by dgascon           #+#    #+#             */
/*   Updated: 2020/02/17 09:19:22 by dgascon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_putnbr_format_rec(long n, int count, int fd)
{
	if (n < 0)
	{
		ft_putchar_fd('-', fd);
		n *= -1;
	}
	if (n > 9)
		ft_putnbr_format_rec(n / 10, count + 1, fd);
	ft_putchar_fd((n % 10) + '0', fd);
	if (count % 3 == 1 && count != 1)
		ft_putchar_fd(',', fd);
}

void		ft_putnbr_format_fd(long n, int fd)
{
	ft_putnbr_format_rec(n, 1, fd);
}
