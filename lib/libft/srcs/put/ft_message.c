/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_message.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgascon <dgascon@student.le-101.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/17 14:53:22 by dgascon           #+#    #+#             */
/*   Updated: 2020/02/17 16:48:18 by dgascon          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_type_message(enum e_type_message e)
{
	char *typemsg[3];

	typemsg[0] = "INFO";
	typemsg[1] = "ERROR";
	typemsg[2] = "WARNING";
	return (typemsg[e]);
}

int			ft_msg(enum e_type_message e, char *message, int ret, char *colors)
{
	char *typemsg;

	typemsg = ft_type_message(e);
	ft_printf(WHITE "[");
	if (!ft_strcmp(typemsg, "INFO"))
		ft_printf(BOLDORANGE "");
	else if (!ft_strcmp(typemsg, "ERROR"))
		ft_printf(BOLDRED "");
	else if (!ft_strcmp(typemsg, "WARNING"))
		ft_printf(BOLDYELLOW "");
	ft_printf("%s", typemsg);
	ft_printf(RESET "] ");
	ft_printf("%s%s\n" RESET, colors, message);
	return (ret);
}
