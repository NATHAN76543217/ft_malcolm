/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libftgnl.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sebastienlecaille <sebastienlecaille@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/20 18:32:02 by dgascon           #+#    #+#             */
/*   Updated: 2021/08/01 22:09:24 by sebastienle      ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFTGNL_H
# define LIBFTGNL_H
# define GNL_ERROR -1
# define GNL_SUCCESS 1
# define GNL_ENDFILE 0
# include <fcntl.h>
//Pour open

typedef struct	s_gnl_lc
{
	int				fd;
	char			*buffer;
	struct s_gnl_lc	*next;
}				t_gnl_lc;

typedef struct	s_gnl
{
	int			fd;
	char		*line;
	int			ret;
}				t_gnl;

int				gnl_line(char *buffer, char **line, int index);
int				get_next_line(int fd, char **line);
int				get_next_xbytes(int fd, void **bytes, unsigned int x);
int				checkfileformat(char *file, t_gnl *gnl, char *ext);
#endif
