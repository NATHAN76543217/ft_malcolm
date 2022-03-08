/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libftnumbers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlecaill <nlecaill@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/20 18:32:02 by dgascon           #+#    #+#             */
/*   Updated: 2020/02/17 09:18:19 by dgascon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFTNUMBERS_H
# define LIBFTNUMBERS_H

int				ft_atoi(const char *str);
int             ft_natoi(const char *str, int n);
char			*ft_itoa(int n);
char			*ft_itoa_base(int nb, char *charset);
int				ft_digit(long num);
int				ft_digit_base(long num, char *base);
int				ft_digit_ul_base(unsigned long num, char *base);
int				ft_digit_format_base(long num, char *base);
#endif
