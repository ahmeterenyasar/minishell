/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:50:15 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 11:50:17 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_exit_no_args(t_shell_data *shell)
{
	shell->should_exit = 1;
	return (get_exit_status(shell));
}

static int	handle_exit_too_many_args(t_shell_data *shell)
{
	write(STDERR_FILENO, "exit: too many arguments\n", 25);
	set_exit_status(shell, 1);
	return (1);
}

static int	handle_exit_invalid_arg(char *arg, t_shell_data *shell)
{
	write(STDERR_FILENO, "exit: ", 6);
	write(STDERR_FILENO, arg, ft_strlen(arg));
	write(STDERR_FILENO, ": numeric argument required\n", 28);
	set_exit_status(shell, 2);
	shell->should_exit = 1;
	return (2);
}

static char	*remove_quotes_if_quoted(const char *str)
{
	int		len;
	char	*result;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (len >= 2 && ((str[0] == '"' && str[len - 1] == '"')
			|| (str[0] == '\'' && str[len - 1] == '\'')))
	{
		result = ft_substr(str, 1, len - 2);
		return (result);
	}
	return (ft_strdup(str));
}

static int	handle_exit_with_code(char *arg, t_shell_data *shell)
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
