/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_input_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:51:24 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 12:05:19 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern sig_atomic_t	g_signal;

static int	handle_readline_result(char **input, t_shell_data *shell)
{
	if (!*input)
	{
		if (g_signal == SIGINT)
		{
			handle_sigint_in_readline(shell, input);
			g_signal = 0;
			return (1);
		}
		printf("exit\n");
		return (-1);
	}
	return (0);
}

static int	handle_post_readline_checks(char **input, t_shell_data *shell)
{
	if (g_signal == SIGINT)
	{
		handle_sigint_after_readline(shell, *input);
		g_signal = 0;
		return (1);
	}
	if (!**input)
	{
		free(*input);
		return (1);
	}
	return (0);
}

int	get_user_input(char **input, t_shell_data *shell)
{
	int		result;
	char	*multiline_input;

	g_signal = 0;
	*input = readline("minishell$ ");
	result = handle_readline_result(input, shell);
	if (result != 0)
		return (result);
	result = handle_post_readline_checks(input, shell);
	if (result != 0)
		return (result);
	multiline_input = read_multiline_input(*input, shell);
	if (!multiline_input)
	{
		free(*input);
		if (g_signal == SIGINT)
		{
			g_signal = 0;
			return (1);
		}
		return (-1);
	}
	free(*input);
	*input = multiline_input;
	return (0);
}
