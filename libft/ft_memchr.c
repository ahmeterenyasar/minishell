/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 10:31:11 by ayasar            #+#    #+#             */
/*   Updated: 2024/10/27 15:32:15 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*m;

	m = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		if (m[i] == (unsigned char)c)
			return ((void *)s + (i * sizeof(char)));
		i++;
	}
	return (0);
}
