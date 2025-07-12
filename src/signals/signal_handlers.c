/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raktas <raktas@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:55:19 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/12 10:02:20 by raktas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sigint_interactive(int signo)
{
	(void)signo;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_sigint_executing(int signo)
{
	(void)signo;
	g_signal = SIGINT;
	write(STDOUT_FILENO, "\n", 1);
}

void	handle_sigquit_executing(int signo)
{
	(void)signo;
	g_signal = SIGQUIT;
	write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
}

void	handle_sigint_heredoc(int signo)
{
	(void)signo;
	g_signal = SIGINT;
	close(STDIN_FILENO);
	write(STDOUT_FILENO, "\n", 1);
}
