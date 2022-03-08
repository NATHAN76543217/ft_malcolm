# include "libftendian.h"

/*
** The ft_endian() function tests for endianess of your hardware.
**return LITTLE_ENDIAN ou BIG_ENDIAN
*/

int ft_endian(void)
{
    int		i;
    char	*p;

	i = 1;
	p = (char *)&i;
    if (p[0] == 1)
        return LITTLE_ENDIAN;
    else
        return BIG_ENDIAN;
}
