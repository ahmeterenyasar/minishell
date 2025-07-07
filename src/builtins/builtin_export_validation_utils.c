/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_validation_utils.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:50:31 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 11:50:32 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*export_duplicate_argument(char *arg)
{
	char	*temp;

	temp = ft_strdup(arg);
	if (!temp)
		return (NULL);
	return (temp);
}

int	export_validate_identifier(char *name, char *arg)
{
	if (!is_valid_identifier(name))
		return (export_validate_and_report_error(arg));
	return (0);
}

int	export_check_assignment_format(char *arg)
{
	char	*equals;

	equals = ft_strchr(arg, '=');
	if (!equals)
		return (0);
	return (1);
}
