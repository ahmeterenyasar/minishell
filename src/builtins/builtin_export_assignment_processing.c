/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_assignment_processing.c             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:50:18 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 11:50:19 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	export_parse_assignment(char *temp, char **name, char **value)
{
	char	*equals;

	equals = ft_strchr(temp, '=');
	if (!equals)
		return (0);
	*equals = '\0';
	*name = temp;
	*value = equals + 1;
	return (1);
}

static int	export_process_validated_assignment(char *name, char *value, 
	char *arg, t_shell_data *shell)
{
	if (export_validate_identifier(name, arg))
		return (1);
	if (set_env_variable(shell, name, value) == -1)
		return (export_report_memory_error());
	return (0);
}

int	export_handle_assignment_processing(char *arg, t_shell_data *shell)
{
	char	*temp;
	char	*name;
	char	*value;
	int		result;

	temp = export_duplicate_argument(arg);
	if (!temp)
		return (export_report_memory_error());
	if (!export_parse_assignment(temp, &name, &value))
	{
		free(temp);
		return (0);
	}
	result = export_process_validated_assignment(name, value, arg, shell);
	free(temp);
	return (result);
}
