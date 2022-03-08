#include "libft.h"

/*
** The ft_natoi() function converts the initial portion of the string pointed
** to by str to int representation but will interprete at most n caracters.
*/

int	ft_natoi(const char *str, int n)
{
	int		i;
	int		sign;
	long	result;

	i = 0;
	sign = 1;
	result = 0;
    if (!str)
        return (0);
	while ((*(str + i) == ' ' || (*(str + i) >= '\t' && *(str + i) <= '\r')) && i < n)
		i++;
	if ((*(str + i) == '+' || *(str + i) == '-') && i < n)
		(*(str + i++) == '-') ? sign = -1 : 0;
	while (ft_isdigit(*(str + i)) && i < n)
	{
		if (result * sign > 2147483647)
			return (-1);
		if (result * sign < -2147483648)
			return (0);
		result = result * 10 + (*(str + i++) - '0');
	}
	result *= sign;
	return (result);
}
