/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 17:37:26 by ayasar            #+#    #+#             */
/*   Updated: 2024/10/26 10:39:42 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static long	ft_len(long src)
{
	int	i;

	i = 0;
	if (src < 0)
	{
		src *= -1;
		i++;
	}
	while (src > 0)
	{
		src /= 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	char	*ret;
	int		i;
	long	num;

	if (n == 0)
		return (ft_strdup("0"));
	num = n;
	i = ft_len(num);
	ret = malloc(i + 1);
	if (ret == NULL)
		return (NULL);
	if (n < 0)
	{
		num *= -1;
		ret[0] = '-';
	}
	ret[i] = '\0';
	i--;
	while (i >= 0 && num > 0)
	{
		ret[i] = (num % 10) + '0';
		num /= 10;
		i--;
	}
	return (ret);
}
