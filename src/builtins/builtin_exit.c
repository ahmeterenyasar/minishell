/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:50:15 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 18:54:42 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_exit(char **args, t_shell_data *shell)
{
	write(STDOUT_FILENO, "exit\n", 5);
	if (!args[1])
		return (handle_exit_no_args(shell));
	if (args[2])
		return (handle_exit_too_many_args(shell));
	if (!is_numeric_string(args[1]))
		return (handle_exit_invalid_arg(args[1], shell));
	return (handle_exit_with_code(args[1], shell));
}
