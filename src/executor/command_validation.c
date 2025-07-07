/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_validation.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:52:09 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 11:52:10 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	validate_command_args(t_command *cmd)
{
	if (!cmd->args || !cmd->args[0])
		return (0);
	if (cmd->args[0] && *cmd->args[0] == '\0')
		return (-1);
	if (cmd->args[0] && is_all_whitespace(cmd->args[0]))
		return (-1);
	return (1);
}

void	print_command_not_found_error(char *cmd_name)
{
	write(STDERR_FILENO, "minishell: ", 11);
	write(STDERR_FILENO, cmd_name, ft_strlen(cmd_name));
	if (ft_strchr(cmd_name, '/'))
		write(STDERR_FILENO, ": No such file or directory\n", 28);
	else
		write(STDERR_FILENO, ": command not found\n", 20);
}

void	print_directory_error(char *cmd_name)
{
	write(STDERR_FILENO, "minishell: ", 11);
	write(STDERR_FILENO, cmd_name, ft_strlen(cmd_name));
	write(STDERR_FILENO, ": Is a directory\n", 17);
}

void	print_permission_error(char *cmd_name)
{
	write(STDERR_FILENO, "minishell: ", 11);
	write(STDERR_FILENO, cmd_name, ft_strlen(cmd_name));
	write(STDERR_FILENO, ": Permission denied\n", 20);
}
