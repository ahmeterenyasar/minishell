/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:55:21 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 11:55:22 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>

static void	set_signal(int sig, void (*handler)(int))
{
	signal(sig, handler);
}

void	setup_signals_interactive(void)
{
	set_signal(SIGINT, handle_sigint_interactive);
	set_signal(SIGQUIT, SIG_IGN);
}

void	setup_signals_executing(void)
{
	set_signal(SIGINT, handle_sigint_executing);
	set_signal(SIGQUIT, handle_sigquit_executing);
}

void	setup_signals_heredoc(void)
{
	set_signal(SIGINT, handle_sigint_heredoc);
	set_signal(SIGQUIT, SIG_IGN);
}

void	reset_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
