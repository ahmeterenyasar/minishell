/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   concat_loop_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 16:30:00 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 15:19:34 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_init_data(t_expand_init_data *init_data,
						const char *str,
						t_shell_data *shell)
{
	init_data->str = str;
	init_data->shell = shell;
}

void	handle_loop_iteration(t_var_expand_params *params, int *i, int *j)
{
	if (check_dollar_expansion(params->str, *i))
	{
		process_variable_expansion(params);
		*i = *(params->i);
		*j = *(params->j);
	}
	else if (params->str[*i] == '\x01')
	{
		(*i)++;
	}
	else
	{
		copy_regular_char(params->result, params->j, params->str[*i]);
		(*i)++;
		*j = *(params->j);
	}
}
