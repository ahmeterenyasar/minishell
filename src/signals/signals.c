/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:55:24 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 11:55:25 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_signals(int context)
{
	if (context == INTERACTIVE_MODE)
		setup_signals_interactive();
	else if (context == EXECUTING_MODE)
		setup_signals_executing();
	else if (context == HEREDOC_MODE)
		setup_signals_heredoc();
}
