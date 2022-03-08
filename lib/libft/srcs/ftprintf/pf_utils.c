/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgascon <dgascon@student.le-101.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/12 13:15:36 by dgascon           #+#    #+#             */
/*   Updated: 2020/02/17 09:19:22 by dgascon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	pf_prec(t_pf *tpf, long val, int argsize)
{
	int length;

	length = argsize;
	if (tpf->fprecision && tpf->specifier == 's')
	{
		(tpf->vprecision == -1) ? tpf->vprecision = 0 : 0;
		return (tpf->vprecision < argsize) ? tpf->vprecision : argsize;
	}
	else if (ft_charstr(tpf->specifier, "duixXp"))
	{
		length = tpf->width;
		length -= (tpf->vprecision <= argsize) ? argsize : tpf->vprecision;
		length -= (tpf->width > tpf->vprecision &&
					tpf->vprecision >= argsize && val < 0) ? 1 : 0;
	}
	return (length);
}
