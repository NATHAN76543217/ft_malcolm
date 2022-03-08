/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_disp_perc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgascon <dgascon@student.le-101.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/16 16:55:41 by dgascon           #+#    #+#             */
/*   Updated: 2020/02/17 09:19:22 by dgascon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	disp_percent(t_pf *tpf)
{
	int nb_space;

	nb_space = (tpf->width > 0) ? tpf->width - 1 : tpf->width;
	tpf->length += nb_space + 1;
	if (!tpf->fmoins)
	{
		while (nb_space--)
		{
			if (tpf->fzero || tpf->zero == -1)
				ft_putchar_fd('0', 1);
			else
				ft_putchar_fd(' ', 1);
		}
		ft_putchar_fd('%', 1);
	}
	else
	{
		ft_putchar_fd('%', 1);
		ft_putcharec_fd(' ', nb_space, 1);
	}
}
