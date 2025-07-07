/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:53:00 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 15:06:56 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_dollar_sign(t_env_expand_data *data)
{
	char	*value;

	(*(data->i))++;
	*(data->i) = extract_env_name(data->str, *(data->i), data->var_name, 256);
	value = get_env_value(data->var_name, data->shell);
	copy_env_value(data->result, data->j, value);
}

void	expand_loop(const char *str, char *result, char *var_name,
		t_shell_data *shell)
{
	t_env_expand_data	data;
	int					i;
	int					j;

	i = 0;
	j = 0;
	data.str = str;
	data.i = &i;
	data.j = &j;
	data.result = result;
	data.var_name = var_name;
	data.shell = shell;
	while (str[i] && j < 4095)
	{
		if (check_dollar_expansion(str, i))
		{
			process_dollar_sign(&data);
		}
		else
		{
			copy_regular_char(result, &j, str[i]);
			i++;
		}
	}
	result[j] = '\0';
}

char	*expand_env_vars(const char *str, t_shell_data *shell)
{
	char	*result;
	char	var_name[256];

	if (!str)
		return (ft_strdup(""));
	result = allocate_result_buffer();
	if (!result)
		return (ft_strdup(""));
	expand_loop(str, result, var_name, shell);
	return (result);
}
