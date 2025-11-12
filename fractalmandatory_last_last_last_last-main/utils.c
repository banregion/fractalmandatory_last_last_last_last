#include "fract.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

double	is_it_int(const char **s)
{
	double	res;

	res = 0.0;
	while (**s >= '0' && **s <= '9')
	{
		res = res * 10 + (**s - '0');
		(*s)++;
	}
	return (res);
}

double	is_it_double(const char **s)
{
	double	fraction;
	double	divisor;

	fraction = 0.0;
	divisor = 1.0;
	while (**s >= '0' && **s <= '9')
	{
		fraction = fraction * 10 + (**s - '0');
		divisor *= 10;
		(*s)++;
	}
	return (fraction / divisor);
}

double	ft_atof(const char *str)
{
	double	res;
	int		sign;

	res = 0.0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
		if (*str++ == '-')
			sign = -1;
	res = is_it_int(&str);
	if (*str == '.' || *str == ',')
	{
		str++;
		res += is_it_double(&str);
	}
	return (res * sign);
}