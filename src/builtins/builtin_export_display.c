/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_display.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:50:22 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 11:50:23 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_export_var_with_value(char *var)
{
	char	*equals;

	write(STDOUT_FILENO, "declare -x ", 11);
	equals = ft_strchr(var, '=');
	*equals = '\0';
	write(STDOUT_FILENO, var, ft_strlen(var));
	write(STDOUT_FILENO, "=\"", 2);
	write(STDOUT_FILENO, equals + 1, ft_strlen(equals + 1));
	write(STDOUT_FILENO, "\"\n", 2);
	*equals = '=';
}

static void	print_export_var_without_value(char *var)
{
	write(STDOUT_FILENO, "declare -x ", 11);
	write(STDOUT_FILENO, var, ft_strlen(var));
	write(STDOUT_FILENO, "\n", 1);
}

static void	print_sorted_vars(char **sorted)
{
	char	*equals;
	int		i;

	i = 0;
	while (sorted[i])
	{
		equals = ft_strchr(sorted[i], '=');
		if (equals)
			print_export_var_with_value(sorted[i]);
		else
			print_export_var_without_value(sorted[i]);
		i++;
	}
}

void	print_export_vars(char **envp)
{
	char	**sorted;

	sorted = sort_envp_for_export(envp);
	if (!sorted)
		sorted = envp;
	print_sorted_vars(sorted);
	if (sorted != envp)
		free(sorted);
}
