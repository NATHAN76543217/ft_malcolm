/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sebastienlecaille <sebastienlecaille@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/12 15:56:28 by nlecaill          #+#    #+#             */
/*   Updated: 2021/08/01 22:10:32 by sebastienle      ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		gnl_line(char *buffer, char **line, int index)
{
	if (index > 0)
		(*line) = ft_substr(buffer, 0, index - 1);
	else
		(*line) = ft_substr(buffer, 0, ft_strlen(buffer));
	if (!(*line))
		return (GNL_ERROR);
	return (0);
}

int	checkfileformat(char *file, t_gnl *gnl, char *ext)
{
	if (ft_strcmp(file + ft_strlen(file) - 4, ext) != 0)
		return (ft_msg(TM_ERROR, "Extention du fichier, non correct.",
				EXIT_FAILURE, RED));
	if (((*gnl).fd = open(file, O_RDONLY)) <= 0)
		return (ft_msg(TM_ERROR, "Ouverture du fichier impossible.",
				EXIT_FAILURE, RED));
	return (EXIT_SUCCESS);
}