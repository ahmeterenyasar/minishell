/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 16:40:34 by ayasar            #+#    #+#             */
/*   Updated: 2024/10/26 16:19:03 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	j;
	char	*str;

	if (s)
	{
		if (start >= ft_strlen(s) || len == 0 || ft_strlen(s) == 0)
			return (ft_strdup(""));
		i = 0;
		while (i < len && s[i + start])
			i++;
		str = (char *) malloc((sizeof(char) * i) + 1);
		if (!(str))
			return (NULL);
		j = 0;
		while (j < i)
		{
			str[j] = s[start + j];
			j++;
		}
		str[j] = '\0';
		return (str);
	}
	return (NULL);
}
