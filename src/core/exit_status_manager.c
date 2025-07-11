/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status_manager.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:51:15 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/12 01:58:31 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_exit_status(t_shell_data *shell, int status)
{
	if (!shell)
		return ;
	shell->last_exit_status = status & 0xFF;
}

int	get_exit_status(t_shell_data *shell)
{
	if (!shell)
		return (0);
	return (shell->last_exit_status);
}
