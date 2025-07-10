/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:53:03 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/10 15:58:22 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	copy_env_value(char *result, int *j, char *value)
{
	int	k;

	if (!value)
		return ;
	k = 0;
	while (value[k] && *j < 4095)
	{
		result[(*j)++] = value[k++];
	}
	free(value);
}

static int	is_valid_var_char(char c)
{
	if (ft_isalnum(c))
		return (1);
	if (c == '_')
		return (1);
	if (c == '?' || c == '$')
		return (1);
	return (0);
}

int	check_dollar_expansion(const char *str, int i)
{
	if (str[i] != '$')
		return (0);
	if (!str[i + 1])
		return (0);
	return (is_valid_var_char(str[i + 1]));
}

char	*allocate_result_buffer(void)
{
	char	*result;

	result = malloc(4096);
	if (!result)
		return (ft_strdup(""));
	return (result);
}

void	copy_regular_char(char *result, int *j, char c)
{
	if (*j < 4095)
		result[(*j)++] = c;
}
