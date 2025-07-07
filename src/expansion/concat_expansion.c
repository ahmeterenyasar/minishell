/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   concat_expansion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:52:58 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 13:52:01 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_variable_expansion(t_var_expand_params *params)
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

static void	init_expand_params(t_var_expand_params *params, const char *str,
		char *result, char *var_name, t_shell_data *shell, int *i, int *j)
{
	params->str = str;
	params->i = i;
	params->var_name = var_name;
	params->shell = shell;
	params->result = result;
	params->j = j;
}

static void	process_expand_loop(t_var_expand_params *params)
{
	int	i;
	int	j;

	i = *(params->i);
	j = *(params->j);
	while (params->str[i] && j < 4095)
	{
		if (check_dollar_expansion(params->str, i))
		{
			process_variable_expansion(params);
			i = *(params->i);
			j = *(params->j);
		}
		else if (params->str[i] == '\x01')
		{
			i++;
		}
		else
		{
			copy_regular_char(params->result, params->j, params->str[i]);
			i++;
			j = *(params->j);
		}
	}
	*(params->i) = i;
	*(params->j) = j;
}

static void	concat_expand_loop(const char *str, char *result, char *var_name,
		t_shell_data *shell)
{
	t_var_expand_params	params;
	int					i;
	int					j;

	i = 0;
	j = 0;
	init_expand_params(&params, str, result, var_name, shell, &i, &j);
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
