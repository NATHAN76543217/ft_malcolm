/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_disp_ptr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgascon <dgascon@student.le-101.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/16 15:38:37 by dgascon           #+#    #+#             */
/*   Updated: 2020/02/17 09:19:22 by dgascon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	disp_ptr_2(t_pf *tpf, char *base, unsigned long val)
{
	if (tpf->fmoins == FALSE)
	{
		ft_putcharec_fd(' ', tpf->whitespace, 1);
		(tpf->specifier == 'p') ? ft_putstr_fd("0x", 1) : 0;
		ft_putcharec_fd('0', tpf->zero, 1);
		if (!(tpf->fprecision == 1 && val == 0 && tpf->vprecision <= 0))
			ft_putnbr_ul_base_fd(val, 1, base);
	}
	else
	{
		(tpf->specifier == 'p') ? ft_putstr_fd("0x", 1) : 0;
		ft_putcharec_fd('0', tpf->zero, 1);
		if (!(tpf->fprecision == 1 && val == 0 && tpf->vprecision <= 0))
			ft_putnbr_ul_base_fd(val, 1, base);
		ft_putcharec_fd(' ', tpf->whitespace, 1);
	}
}

void		disp_ptr(t_pf *tpf, char *base)
{
	unsigned long	val;
	int				argsize;

	val = va_arg(*(tpf->ap), unsigned long);
	argsize = ft_digit_ul_base(val, base);
	(tpf->specifier == 'p') ? argsize += 2 : 0;
	tpf->zero = (tpf->fprecision) ? tpf->vprecision - argsize : 0;
	(tpf->vprecision > argsize) ? tpf->zero++ : 0;
	tpf->whitespace = pf_prec(tpf, val, argsize);
	if (tpf->fprecision == 1 && val == 0 && tpf->vprecision <= 0)
	{
		tpf->length--;
		tpf->whitespace++;
	}
	if (tpf->zero && tpf->width > tpf->vprecision)
	{
		(tpf->whitespace > 0) ? tpf->zero += tpf->whitespace : 0;
		tpf->whitespace = 0;
	}
	length_calc(tpf, 3, tpf->whitespace, tpf->zero, argsize);
	disp_ptr_2(tpf, base, val);
}
