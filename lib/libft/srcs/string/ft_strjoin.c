/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgascon <dgascon@student.le-101.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/21 14:53:41 by dgascon           #+#    #+#             */
/*   Updated: 2020/02/17 10:05:40 by dgascon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*ptr;
	size_t	sizes1;
	size_t	sizes2;
	size_t	i;

	if (s1 == 0 && s2 != 0)
		return (ft_strdup(s2));
	if (s2 == 0 && s1 != 0)
		return (ft_strdup(s1));
	sizes1 = ft_strlen(s1);
	sizes2 = ft_strlen(s2);
	if ((s1 == 0 && s2 == 0)
	|| !(ptr = wrmalloc((sizes1 + sizes2 + 1) * sizeof(char))))
		return (NULL);
	i = -1;
	while (++i < sizes1)
		ptr[i] = s1[i];
	i = -1;
	while (++i < sizes2)
		ptr[i + sizes1] = s2[i];
	ptr[sizes2 + sizes1] = '\0';
	return (ptr);
}

char	*ft_strmjoin(char *format, ...)
{
	va_list ap;
	char	*tmp;
	char	*ret;

	tmp = NULL;
	ret = ft_strdup("");
	if (!format)
		return (NULL);
	va_start(ap, format);
	while (*format)
	{
		if (*format == 's')
			tmp = ft_strjoin(ret, va_arg(ap, char *));
		else if (*format == 'd')
			tmp = ft_strjoin(ret, ft_itoa(va_arg(ap, int)));
		else if (*format == 'c')
			tmp = ft_strjoin(ret, ft_chartostr(va_arg(ap, int)));
		else
			return (NULL);
		wrfree(ret);
		ret = tmp;
		format++;
	}
	va_end(ap);
	return (ret);
}
