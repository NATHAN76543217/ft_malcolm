/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sebastienlecaille <sebastienlecaille@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/12 15:28:45 by nlecaill          #+#    #+#             */
/*   Updated: 2021/08/01 22:11:00 by sebastienle      ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 64
#endif

static t_gnl_lc	*gnl_new(int fd, char *buffer)
{
	t_gnl_lc *current;

	if (!(current = wrmalloc(sizeof(t_gnl_lc))))
		return (NULL);
	if (!buffer)
	{
		if (!(current->buffer = ft_substr("", 0, 1)))
		{
			wrfree(current);
			return (NULL);
		}
	}
	else
		current->buffer = buffer;
	current->fd = fd;
	current->next = NULL;
	return (current);
}

static t_gnl_lc	*gnl_fct(t_gnl_lc **lsts, int fd)
{
	t_gnl_lc *current;

	if (!(current = (*lsts)))
		return (((*lsts) = gnl_new(fd, NULL)));
	while (current)
	{
		if (current->fd == fd)
			break ;
		else if (!(current->next))
		{
			current->next = gnl_new(fd, NULL);
			return (current->next);
		}
		(current) = (current)->next;
	}
	return (current);
}

static int		gnl_check(int fd, char **buffer)
{
	char	*str;
	int		read_size;
	int		state;
	char	*buffer_save;

	state = 1;
	if (!(str = wrmalloc((BUFFER_SIZE + 1) * sizeof(char))))
		return (GNL_ERROR);
	if ((read_size = read(fd, str, BUFFER_SIZE)) == -1)
	{
		wrfree(str);
		return (GNL_ERROR);
	}
	str[read_size] = '\0';
	(!ft_strlen(str)) ? state = 0 : 0;
	buffer_save = *buffer;
	if (!(*buffer = ft_strjoin(*buffer, str)))
	{
		wrfree(str);
		return (GNL_ERROR);
	}
	wrfree(buffer_save);
	wrfree(str);
	return (state);
}

static int		gnl_free_mail(t_gnl_lc **buffer, t_gnl_lc *current, int ret)
{
	t_gnl_lc *tmp;
	t_gnl_lc *tmp_prev;

	tmp_prev = NULL;
	if ((tmp = (*buffer)) == current)
		(current->next) ? (*buffer) = current->next : 0;
	while (tmp)
	{
		if (tmp == current)
		{
			if (tmp_prev != NULL)
				tmp_prev->next = tmp->next;
			if (current == (*buffer))
				(*buffer) = NULL;
			wrfree(current);
			current = NULL;
			break ;
		}
		tmp_prev = tmp;
		tmp = tmp->next;
	}
	return (ret);
}

int				get_next_line(int fd, char **line)
{
	static t_gnl_lc	*buffer;
	t_gnl_lc		*current;
	char			*n_buff;
	int				i;
	int				state_gnlcheck;

	n_buff = NULL;
	if (!(fd >= 0 && BUFFER_SIZE > 0 && (current = gnl_fct(&buffer, fd))))
		return (GNL_ERROR);
	while (!(i = ft_strichr(current->buffer, '\n')))
		if ((state_gnlcheck = gnl_check(fd, &current->buffer)) == -1)
			return (GNL_ERROR);
		else if (state_gnlcheck == 0)
			break ;
	if ((i > 0) &&
		!(n_buff = ft_substr(current->buffer, i, ft_strlen(current->buffer))))
		return (GNL_ERROR);
	if (gnl_line(current->buffer, line, i) == -1)
		return (GNL_ERROR);
	wrfree(current->buffer);
	current->buffer = n_buff;
	return ((i == 0) ? gnl_free_mail(&buffer, current, GNL_ENDFILE) : GNL_SUCCESS);
}
