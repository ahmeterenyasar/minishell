/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   concat_expansion_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:52:55 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 15:22:56 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	skip_leading_whitespace(t_var_expand_params *params)
{
	char	c;

	c = params->str[*(params->i)];
	while (c && (c == ' ' || c == '\t'))
	{
		copy_regular_char(params->result, params->j, c);
		(*(params->i))++;
		c = params->str[*(params->i)];
	}
}

void	handle_special_vars(t_var_expand_params *params)
{
	char	*value;
	int		k;

	params->var_name[0] = params->str[*(params->i)];
	params->var_name[1] = '\0';
	(*(params->i))++;
	value = get_env_value(params->var_name, params->shell);
	if (value)
	{
		k = 0;
		while (value[k] && *(params->j) < 4095)
			params->result[(*(params->j))++] = value[k++];
		free(value);
	}
}

int	calculate_var_name_length(t_var_expand_params *params)
{
	int	name_len;

	name_len = 0;
	while (params->str[*(params->i) + name_len] && params->str[*(params->i)
			+ name_len] != '\x01')
	{
		if (!ft_isalnum(params->str[*(params->i) + name_len])
			&& params->str[*(params->i) + name_len] != '_')
			break ;
		if (name_len >= 255)
			break ;
		name_len++;
	}
	return (name_len);
}

void	process_var_value(t_var_expand_params *params, int name_len)
{
	char	*value;
	int		k;

	ft_strncpy(params->var_name, params->str + *(params->i), name_len);
	params->var_name[name_len] = '\0';
	value = get_env_value(params->var_name, params->shell);
	if (value)
	{
		k = 0;
		while (value[k] && *(params->j) < 4095)
			params->result[(*(params->j))++] = value[k++];
		free(value);
	}
	*(params->i) += name_len;
}

void	handle_regular_vars(t_var_expand_params *params)
{
	int	name_len;

	name_len = calculate_var_name_length(params);
	if (name_len > 0)
		process_var_value(params, name_len);
	else
		copy_regular_char(params->result, params->j, '$');
}
