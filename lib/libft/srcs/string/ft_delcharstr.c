/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_delcharstr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgascon <dgascon@student.le-101.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/27 17:26:06 by dgascon           #+#    #+#             */
/*   Updated: 2020/02/17 09:19:22 by dgascon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_delcharstr(char *line, char *charset)
{
	int		i;
	char	*tmp;
	char	*linechar;
	char	*ret;

	i = 0;
	tmp = NULL;
	while (line[i])
	{
		if (!(ft_charstr(line[i], charset)))
		{
			linechar = ft_chartostr(line[i]);
			ret = ft_strjoin(tmp, linechar);
			wrfree(linechar);
			wrfree(tmp);
			tmp = ret;
		}
		i++;
	}
	return (ret);
}
