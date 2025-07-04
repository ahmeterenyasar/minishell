#include "libft.h"

/**
 * String utility functions that are not available in libft
 * or have different implementations needed for minishell
 */

char	*ft_strcpy(char *dest, const char *src)
{
	int i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}