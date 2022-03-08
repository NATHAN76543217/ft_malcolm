/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_disp_string.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgascon <dgascon@student.le-101.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/16 15:35:23 by dgascon           #+#    #+#             */
/*   Updated: 2020/02/17 09:19:22 by dgascon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	disp_str(t_pf *tpf)
{
	int				argsize;
	char			*value;
	int				tronc;

	if (!(value = va_arg(*(tpf->ap), char *)))
		value = "(null)";
	if (tpf->fprecision && tpf->vprecision == -1)
		argsize = 0;
	else
		argsize = ft_strlen(value);
	tronc = pf_prec(tpf, 0, argsize);
	(tpf->width > 0) ? tpf->whitespace = tpf->width - tronc : 0;
	length_calc(tpf, 2, tronc, tpf->whitespace);
	if (tpf->fmoins == FALSE)
	{
		ft_putcharec_fd(' ', tpf->whitespace, 1);
		ft_putnstr_fd(value, tronc, 1);
	}
	else
	{
		ft_putnstr_fd(value, tronc, 1);
		ft_putcharec_fd(' ', tpf->whitespace, 1);
	}
}
