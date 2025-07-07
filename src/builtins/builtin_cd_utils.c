/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:49:57 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 12:03:33 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	validate_cd_args(char **args, t_shell_data *shell)
{
	if (args[1] && args[2])
	{
		write(STDERR_FILENO, "cd: too many arguments\n", 23);
		set_exit_status(shell, 1);
		return (1);
	}
	return (0);
}

char	*get_home_path(t_shell_data *shell, char *old_pwd)
{
	char	*home;

	home = get_env_value("HOME", shell);
	if (!home || !*home)
	{
		write(STDERR_FILENO, "cd: HOME not set\n", 17);
		if (old_pwd)
			free(old_pwd);
		if (home)
			free(home);
		set_exit_status(shell, 1);
		return (NULL);
	}
	return (home);
}

char	*get_oldpwd_path(t_shell_data *shell, char *old_pwd)
{
	char	*path;

	path = get_env_value("OLDPWD", shell);
	if (!path || !*path)
	{
		write(STDERR_FILENO, "cd: OLDPWD not set\n", 19);
		if (old_pwd)
			free(old_pwd);
		if (path)
			free(path);
		set_exit_status(shell, 1);
		return (NULL);
	}
	write(STDOUT_FILENO, path, ft_strlen(path));
	write(STDOUT_FILENO, "\n", 1);
	return (path);
}

int	change_to_directory(char *path, char *old_pwd, int should_free_path,
		t_shell_data *shell)
{
	if (chdir(path) == -1)
	{
		write(STDERR_FILENO, "cd: ", 4);
		perror(path);
		if (old_pwd)
			free(old_pwd);
		if (should_free_path)
			free(path);
		set_exit_status(shell, 1);
		return (1);
	}
	update_pwd_vars(shell, old_pwd);
	if (old_pwd)
		free(old_pwd);
	if (should_free_path)
		free(path);
	set_exit_status(shell, 0);
	return (0);
}

int	determine_cd_path(char **args, t_shell_data *shell, char *old_pwd,
		char **path, int *should_free_path)
{
	if (!args[1] || ft_strcmp(args[1], "~") == 0)
	{
		*path = get_home_path(shell, old_pwd);
		if (!*path)
			return (1);
		*should_free_path = 1;
	}
	else if (ft_strcmp(args[1], "-") == 0)
	{
		*path = get_oldpwd_path(shell, old_pwd);
		if (!*path)
			return (1);
		*should_free_path = 1;
	}
	else
	{
		*path = args[1];
		*should_free_path = 0;
	}
	return (0);
}
