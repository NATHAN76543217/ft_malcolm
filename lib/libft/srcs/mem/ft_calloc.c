/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgascon <dgascon@student.le-101.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/13 12:36:38 by dgascon           #+#    #+#             */
/*   Updated: 2020/02/17 09:19:22 by dgascon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** The ft_calloc() function contiguously allocates enough space for count
** objects that are size bytes of memory each and returns a pointer to the
** allocated memory.
** The allocated memory is filled with bytes of value zero.
*/

void	*ft_calloc(size_t count, size_t size)
{
	void	*m;

	if (!(m = wrmalloc(count * size)))
		return (NULL);
	ft_bzero(m, size * count);
	return (m);
}
