/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_cleanup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:51:20 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 11:51:21 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_envp_if_exists(t_shell_data *shell)
{
	if (shell->envp)
	{
		free_envp(shell->envp);
		shell->envp = NULL;
	}
}

void	free_shell_data(t_shell_data *shell)
{
	if (!shell)
		return ;
	cleanup_shell_readline_data(shell);
	free_envp_if_exists(shell);
	free(shell);
}
