#include "minishell.h"

void	print_error(const char *msg)
{
	write(STDERR_FILENO, msg, ft_strlen(msg));
}

char	*int_to_string(int n)
{
	char	*str;
	int		len;
	int		temp;
	int		i;
	int		is_negative;

	is_negative = 0;
	if (n < 0)
	{
		is_negative = 1;
		if (n == -2147483648) /* Handle INT_MIN special case */
			return (ft_strdup("-2147483648"));
		n = -n;
	}
	
	temp = n;
	len = (n == 0) ? 1 : 0;
	while (temp > 0)
	{
		temp /= 10;
		len++;
	}
	
	if (is_negative)
		len++;
	
	str = malloc(len + 1);
	if (!str)
		return (NULL);
	
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
	
	return (str);
}
