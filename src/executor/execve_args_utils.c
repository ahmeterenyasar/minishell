/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_args_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:52:38 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 14:52:53 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_command_args(t_command *cmd)
{
	int	args_count;

	args_count = 0;
	while (cmd->args[args_count])
		args_count++;
	return (args_count);
}

char	**allocate_args_backup(int args_count, t_shell_data *shell,
		t_pipeline_context *ctx)
{
	char	**args_backup;

	args_backup = malloc(sizeof(char *) * (args_count + 1));
	if (!args_backup)
	{
		cleanup_pipeline_child_memory(shell, ctx);
		exit(1);
	}
	return (args_backup);
}

void	free_partial_args_backup(char **args_backup, int count)
{
	while (--count >= 0)
		free(args_backup[count]);
	free(args_backup);
}

void	copy_args_to_backup(t_command *cmd, char **args_backup, int args_count,
		t_shell_data *shell, t_pipeline_context *ctx)
{
	int	i;

	i = 0;
	while (i < args_count)
	{
		args_backup[i] = ft_strdup(cmd->args[i]);
		if (!args_backup[i])
		{
			free_partial_args_backup(args_backup, i);
			cleanup_pipeline_child_memory(shell, ctx);
			exit(1);
		}
		i++;
	}
	args_backup[args_count] = NULL;
}
