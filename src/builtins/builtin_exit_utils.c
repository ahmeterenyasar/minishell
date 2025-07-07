/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 16:30:00 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 18:54:42 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_exit_no_args(t_shell_data *shell)
{
	shell->should_exit = 1;
	return (get_exit_status(shell));
}

int	handle_exit_too_many_args(t_shell_data *shell)
{
	write(STDERR_FILENO, "exit: too many arguments\n", 25);
	set_exit_status(shell, 1);
	return (1);
}

int	handle_exit_invalid_arg(char *arg, t_shell_data *shell)
{
	write(STDERR_FILENO, "exit: ", 6);
	write(STDERR_FILENO, arg, ft_strlen(arg));
	write(STDERR_FILENO, ": numeric argument required\n", 28);
	set_exit_status(shell, 2);
	shell->should_exit = 1;
	return (2);
}

int	handle_exit_with_code(char *arg, t_shell_data *shell)
{
	long	temp;
	int		exit_code;
	char	*unquoted;

	unquoted = remove_quotes_if_quoted(arg);
	if (!unquoted)
	{
		set_exit_status(shell, 2);
		shell->should_exit = 1;
		return (2);
	}
	temp = ft_atoi(unquoted);
	free(unquoted);
	exit_code = (unsigned char)temp;
	set_exit_status(shell, exit_code);
	shell->should_exit = 1;
	return (exit_code);
}
