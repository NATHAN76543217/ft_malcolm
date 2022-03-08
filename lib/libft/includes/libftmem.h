/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libftmem.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlecaill <nlecaill@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/20 18:32:02 by dgascon           #+#    #+#             */
/*   Updated: 2020/02/17 09:18:16 by dgascon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFTMEM_H
# define LIBFTMEM_H
# include <stddef.h>

void			*ft_memset(void *b, int c, size_t len);
void			*ft_memcpy(void *dst, void const *src, size_t n);
void			*ft_memccpy(void *dst, void const *src, int c, size_t n);
void			*ft_memmove(void *dst, const void *src, size_t len);
void			*ft_memchr(const void *s, int c, size_t n);
int				ft_memcmp(const void *s1, const void *s2, size_t n);
void			ft_bzero(void *s, size_t n);
void			*ft_calloc(size_t count, size_t size);
void			*wrmalloc(unsigned long size);
int				wrfree(void *ptr);
void			wrdestroy(void);
#endif
