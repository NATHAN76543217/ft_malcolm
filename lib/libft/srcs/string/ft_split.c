/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgascon <dgascon@student.le-101.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/23 13:32:33 by dgascon           #+#    #+#             */
/*   Updated: 2020/02/17 09:19:22 by dgascon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		splitfree(char **ptr)
{
	int i;

	i = 0;
	while (ptr[i])
	{
		wrfree(ptr[i]);
		i++;
	}
	wrfree(ptr);
}

static int	splitproc(char **ptr, char const *s, char c)
{
	int	i;
	int	row;
	int	start;
	int	end;

	i = -1;
	row = 0;
	start = -1;
	end = 1;
	while (s[++i])
	{
		if (s[i] != c)
		{
			(start == -1) ? start = i : 0;
			if (s[i + 1] == c || s[i + 1] == '\0')
			{
				if (!(ptr[row++] = ft_substr(s, start, end)))
					return (1);
				start = -1;
				end = 0;
			}
			end++;
		}
	}
	return (0);
}

char		**ft_split(char const *s, char c)
{
	char	**ptr;
	int		rows;
	int		i;

	if (!s)
		return (0);
	i = 0;
	rows = 0;
	while (s[i])
	{
		(s[i] != c && (s[i + 1] == c || s[i + 1] == '\0')) ? rows++ : 0;
		i++;
	}
	if (!(ptr = wrmalloc((rows + 1) * sizeof(char *))))
		return (0);
	ptr[rows] = 0;
	if ((splitproc(ptr, s, c)) != 0)
	{
		splitfree(ptr);
		return (NULL);
	}
	return (ptr);
}
