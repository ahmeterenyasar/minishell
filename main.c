/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:56:12 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 11:56:13 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

sig_atomic_t	g_signal = 0;

static t_shell_data	*initialize_shell(char **envp)
{
	t_shell_data	*shell;

	shell = init_shell_data(envp);
	if (!shell)
		return (NULL);
	setup_signals(INTERACTIVE_MODE);
	return (shell);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell_data	*shell;

	(void)argc;
	(void)argv;
	shell = initialize_shell(envp);
	if (!shell)
		return (1);
	run_shell_main_loop(shell);
	free_shell_data(shell);
	return (0);
}
