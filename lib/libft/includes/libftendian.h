/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libftendian.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlecaill <nlecaill@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/16 15:56:07 by dgascon           #+#    #+#             */
/*   Updated: 2020/02/28 15:52:16 by nlecaill         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFTENDIAN_H
# define LIBFTENDIAN_H

# define LITTLE_ENDIAN 1
# define BIG_ENDIAN 2

int ft_endian(void);
int ft_r_endian_int(int num);

#endif