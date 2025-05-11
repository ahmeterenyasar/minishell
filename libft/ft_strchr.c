/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 16:06:11 by ayasar            #+#    #+#             */
/*   Updated: 2024/10/26 16:18:20 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	unsigned char	d;
	unsigned char	*m;
	size_t			i;

	d = c;
	i = 0;
	m = (unsigned char *)s;
	while (i != (ft_strlen((char *)m) + 1) && m)
	{
		if (s[i] == d)
			return ((char *)(s + (i * sizeof(char))));
		else
			i++;
	}
	return (NULL);
}
