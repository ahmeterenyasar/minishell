/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:50:52 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 12:04:27 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_oldpwd_from_parameter(t_shell_data *shell, char *old_pwd)
{
	if (old_pwd && *old_pwd)
		set_env_variable(shell, "OLDPWD", old_pwd);
}

static void	update_oldpwd_variable(t_shell_data *shell, char *old_pwd)
{
	char	*current_pwd;

	current_pwd = get_env_value("PWD", shell);
	if (current_pwd && *current_pwd)
	{
		set_env_variable(shell, "OLDPWD", current_pwd);
		free(current_pwd);
	}
	else
		set_oldpwd_from_parameter(shell, old_pwd);
}

int	execute_pwd(t_shell_data *shell)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("pwd");
		set_exit_status(shell, 1);
		return (1);
	}
	write(STDOUT_FILENO, cwd, ft_strlen(cwd));
	write(STDOUT_FILENO, "\n", 1);
	free(cwd);
	set_exit_status(shell, 0);
	return (0);
}

void	update_pwd_vars(t_shell_data *shell, char *old_pwd)
{
	char	*new_pwd;

	if (!shell)
		return ;
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
		return ;
	update_oldpwd_variable(shell, old_pwd);
	set_env_variable(shell, "PWD", new_pwd);
	free(new_pwd);
}
