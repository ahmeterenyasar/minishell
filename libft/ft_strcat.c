#include "libft.h"

char	*ft_strcat(char *dest, const char *src)
{
	int dest_len = ft_strlen(dest);
	int i = 0;
	while (src[i])
	{
		dest[dest_len + i] = src[i];
		i++;
	}
	dest[dest_len + i] = '\0';
	return (dest);
}
