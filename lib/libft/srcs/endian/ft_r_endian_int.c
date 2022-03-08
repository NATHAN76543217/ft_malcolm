#include "libftendian.h"

/*
** The ft_r_endian_int() function return the value of num in the opposite endianness.
*/

int ft_r_endian_int(int num)
{
    int		s;
    int		p[4];

	p[0] = (num & 0x000000ff) << 24u;
	p[1] = (num & 0x0000ff00) << 8u;
	p[2] = (num & 0x00ff0000) >> 8u;
	p[3] = (num & 0xff000000) >> 24u;
    s = p[0] | p[1] | p[2] | p[3];

	return (s);
}
