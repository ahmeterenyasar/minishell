/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   concat_expansion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:52:58 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 11:52:59 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_variable_expansion(const char *str, int *i, char *var_name,
		t_shell_data *shell, char *result, int *j)
{
	(*i)++;
	if (str[*i] == '?' || str[*i] == '$')
	{
		handle_special_vars(str, i, var_name, shell, result, j);
	}
	else
	{
		handle_regular_vars(str, i, var_name, shell, result, j);
	}
}

static void	concat_expand_loop(const char *str, char *result, char *var_name,
		t_shell_data *shell)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	skip_leading_whitespace(str, &i, result, &j);
	while (str[i] && j < 4095)
	{
		if (check_dollar_expansion(str, i))
		{
			process_variable_expansion(str, &i, var_name, shell, result, &j);
		}
		else if (str[i] == '\x01')
		{
			i++;
		}
		else
		{
			copy_regular_char(result, &j, str[i]);
			i++;
		}
	}
	result[j] = '\0';
}

char	*expand_concatenated_vars(const char *str, t_shell_data *shell)
{
	char	*result;
	char	var_name[256];

	if (!str)
		return (ft_strdup(""));
	result = allocate_result_buffer();
	if (!result)
		return (ft_strdup(""));
	concat_expand_loop(str, result, var_name, shell);
	return (result);
}
