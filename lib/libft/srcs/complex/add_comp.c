/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_comp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlecaill <nlecaill@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/02 18:35:54 by nlecaill          #+#    #+#             */
/*   Updated: 2020/03/02 18:42:13 by nlecaill         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_f_complex   add_comp(t_f_complex v1, t_f_complex v2)
{
    t_f_complex res;

    res.re = v1.re + v2.re;
    res.im = v1.im + v2.im;
    return (res);
}

t_f_complex   sub_comp(t_f_complex v1, t_f_complex v2)
{
    t_f_complex res;

    res.re = v1.re - v2.re;
    res.im = v1.im - v2.im;
    return (res);
}