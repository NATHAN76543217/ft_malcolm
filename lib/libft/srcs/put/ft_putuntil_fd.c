#include "libftput.h"

/*
** The ft_putuntil_fd() function write str until the limit char is not met.
*/

void	ft_putuntil_fd(int fd, const char *str, char limit)
{
	int i;

	if (!str)
		return ;
	i = 0;
	while (str[i] != limit && str[i] != '\0')
		ft_putchar_fd(str[i++], fd);
}