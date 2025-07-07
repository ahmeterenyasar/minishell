/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   concat_expansion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:52:58 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 17:30:00 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_variable_expansion(t_var_expand_params *params)
{
	(*(params->i))++;
	if (params->str[*(params->i)] == '?' || params->str[*(params->i)] == '$')
	{
		handle_special_vars(params);
	}
	else
	{
		handle_regular_vars(params);
	}
}

static void	init_expand_params(t_var_expand_params *params,
		t_expand_init_data *init_data, int *i, int *j)
{
	params->str = init_data->str;
	params->i = i;
	params->var_name = init_data->var_name;
	params->shell = init_data->shell;
	params->result = init_data->result;
	params->j = j;
}

static void	process_expand_loop(t_var_expand_params *params)
{
	while (params->str[*(params->i)] && *(params->j) < 4095)
	{
		if (check_dollar_expansion(params->str, *(params->i)))
		{
			process_variable_expansion(params);
		}
		else if (params->str[*(params->i)] == '\x01')
		{
			(*(params->i))++;
		}
		else
		{
			copy_regular_char(params->result, params->j,
				params->str[*(params->i)]);
			(*(params->i))++;
		}
	}
}

static void	concat_expand_loop(const char *str, char *result, char *var_name,
		t_shell_data *shell)
{
	t_var_expand_params	params;
	t_expand_init_data	init_data;
	int					i;
	int					j;

	i = 0;
	j = 0;
	setup_init_data(&init_data, str, shell);
	init_data.result = result;
	init_data.var_name = var_name;
	init_expand_params(&params, &init_data, &i, &j);
	skip_leading_whitespace(&params);
	process_expand_loop(&params);
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
