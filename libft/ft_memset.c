/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 11:31:49 by ayasar            #+#    #+#             */
/*   Updated: 2024/10/27 15:10:41 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*str;

	i = 0;
	str = (unsigned char *) s;
	while (i < n)
	{
		str[i] = (unsigned char)c;
		i++;
	}
	return (s);
}

#include <stdio.h>
int main()
{
	int arr[] = {1,2,3,4,5};
	int num = 123;
	char *num_ptr = (char *)&num;

	int i = 0;
	while (i < 5)
	{
		char *arr_ptr = (char *)&arr[i];
		int j = 0;
		while (j < 4)
		{
			ft_memset(&arr_ptr[j], num_ptr[j], 1);
			j++;
		}
		printf("%d\n", arr[i]);
		i++;
	}
	
}