#include "minishell.h"

void	print_error(const char *msg)
{
	write(STDERR_FILENO, msg, ft_strlen(msg));
}

static int	calculate_int_length(int n)
{
	int	len;
	int	temp;

	temp = n;
	if (n == 0)
		len = 1;
	else
		len = 0;
	while (temp > 0)
	{
		temp /= 10;
		len++;
	}
	return (len);
}

static void	fill_int_string(char *str, int n, int len, int is_negative)
{
	int	i;

	str[len] = '\0';
	i = len - 1;
	if (n == 0)
		str[0] = '0';
	else
	{
		while (n > 0)
		{
			str[i--] = (n % 10) + '0';
			n /= 10;
		}
		if (is_negative)
			str[0] = '-';
	}
}

char	*int_to_string(int n)
{
	char	*str;
	int		len;
	int		is_negative;

	is_negative = 0;
	if (n < 0)
	{
		is_negative = 1;
		if (n == -2147483648)
			return (ft_strdup("-2147483648"));
		n = -n;
	}
	len = calculate_int_length(n);
	if (is_negative)
		len++;
	str = malloc(len + 1);
	if (!str)
		return (NULL);
	fill_int_string(str, n, len, is_negative);
	return (str);
}
