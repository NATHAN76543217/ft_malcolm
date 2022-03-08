/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mult_comp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlecaill <nlecaill@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/02 18:40:53 by nlecaill          #+#    #+#             */
/*   Updated: 2020/03/02 18:46:30 by nlecaill         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_f_complex mult_comp(t_f_complex v1, t_f_complex v2)
{
    t_f_complex res;

    res.re = (v1.re * v2.re) - (v1.im * v2.im);
    res.im = (v1.re * v2.im) + (v1.im * v2.re);
    return (res);
}