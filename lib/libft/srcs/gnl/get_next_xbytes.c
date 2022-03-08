/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_xbytes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlecaill <nlecaill@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/12 18:32:15 by nlecaill          #+#    #+#             */
/*   Updated: 2020/02/17 09:19:22 by nlecaill         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int     get_next_xbytes(int fd, void **bytes, unsigned int x)
{
    int ret;

    if (x == 0)
        return (1);
    if (read(fd, NULL, 0) < 0)
        return (-1);
    if (bytes == NULL)
        return (-1);
    *bytes = wrmalloc(x);
    ft_bzero(*bytes, x);
    ret = read(fd, *bytes ,x);
    if (ret == 0)
        return (0);
    else if (ret < 1)
        return (-1);
    return (1);
}
