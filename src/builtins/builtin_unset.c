/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:50:57 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 11:50:58 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_no_arguments(t_shell_data *shell)
{
	set_exit_status(shell, 0);
	return (0);
}

int	execute_unset(char **args, t_shell_data *shell)
{
	int	ret_status;

	if (!args[1])
		return (handle_no_arguments(shell));
	ret_status = process_unset_args(shell, args);
	set_exit_status(shell, ret_status);
	return (ret_status);
}
