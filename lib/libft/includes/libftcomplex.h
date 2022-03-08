/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libftcomplex.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlecaill <nlecaill@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/02 18:47:23 by nlecaill          #+#    #+#             */
/*   Updated: 2020/03/02 18:49:40 by nlecaill         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_COMPLEX
# define LIBFT_COMPLEX

typedef struct	s_f_complex
{
	float re;
	float im;
}				t_f_complex;

t_f_complex     mult_comp(t_f_complex v1, t_f_complex v2);
t_f_complex     add_comp(t_f_complex v1, t_f_complex v2);
t_f_complex     sub_comp(t_f_complex v1, t_f_complex v2);

#endif