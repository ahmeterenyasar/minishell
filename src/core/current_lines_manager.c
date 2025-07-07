/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   current_lines_manager.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:51:13 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 11:51:14 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_existing_lines(t_shell_data *shell)
{
	if (shell->current_lines)
	{
		free_str_array(shell->current_lines);
		shell->current_lines = NULL;
	}
}

void	set_current_lines(t_shell_data *shell, char **lines)
{
	if (!shell)
		return ;
	free_existing_lines(shell);
	shell->current_lines = lines;
}

void	clear_current_lines(t_shell_data *shell)
{
	if (!shell)
		return ;
	free_existing_lines(shell);
}
