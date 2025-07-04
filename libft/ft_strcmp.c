#include "libft.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	unsigned char	*us1;
	unsigned char	*us2;

	// Handle NULL pointers
	if (!s1 && !s2)
		return (0);
	if (!s1)
		return (-1);
	if (!s2)
		return (1);
	us1 = (unsigned char *)s1;
	us2 = (unsigned char *)s2;
	while (*us1 && (*us1 == *us2))
	{
		us1++;
		us2++;
	}
	return (*us1 - *us2);
}
