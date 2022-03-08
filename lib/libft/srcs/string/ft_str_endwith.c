#include "libft.h"

/*
** The ft_str_endwith() function return 1 if str end with buf else return 0.
*/

int     ft_str_endwith(const char *str, const char *buf)
{
	int size;
	int bsize;

	if (!str || !buf)
		return (0);
	size = ft_strlen(str);
	bsize = ft_strlen(buf);
	if (bsize > size)
		return (0);
	while (bsize >= 0)
	{
		if (str[size] != buf[bsize])
			return (0);
		size--;
		bsize--;
	}
	return (1);
}