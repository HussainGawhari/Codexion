#include <limits.h>

static int	ft_isspace(char c)
{
	return ((c >= 9 && c <= 13) || c == ' ');
}

static int	ft_sign(char **str)
{
	int	sign;

	sign = 1;
	if (**str == '-' || **str == '+')
	{
		if (**str == '-')
			sign = -1;
		(*str)++;
	}
	return (sign);
}

int	ft_atoi(const char *str)
{
	int				sign;
	unsigned long	nb;

	while (ft_isspace(*str))
		str++;
	sign = ft_sign((char **)&str);
	nb = 0;
	while (*str >= '0' && *str <= '9')
	{
		nb = nb * 10 + (*str - '0');
		if (sign == 1 && nb > INT_MAX)
			return (-1);
		if (sign == -1 && nb > (unsigned long)INT_MAX + 1)
			return (-1);
		str++;
	}
	return ((int)(nb * sign));
}
