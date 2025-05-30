#include "minishell.h"

int	execute_cd(char **args, t_shell_data *shell)
{
	char	*path;
	char	*home;
	char	*old_pwd;
	int		should_free_path;

	// Too many arguments check
	if (args[1] && args[2])
	{
		write(STDERR_FILENO, "cd: too many arguments\n", 23);
		set_exit_status(shell, 1);
		return (1);
	}

	old_pwd = getcwd(NULL, 0);
	should_free_path = 0;

	if (!args[1] || ft_strcmp(args[1], "~") == 0)
	{
		// Go to home directory
		home = get_env_value("HOME", shell);
		if (!home || !*home)
		{
			write(STDERR_FILENO, "cd: HOME not set\n", 17);
			if (old_pwd)
				free(old_pwd);
			if (home)
				free(home);
			set_exit_status(shell, 1);
			return (1);
		}
		path = home;
		should_free_path = 1;
	}
	else if (ft_strcmp(args[1], "-") == 0)
	{
		// Go to previous directory
		path = get_env_value("OLDPWD", shell);
		if (!path || !*path)
		{
			write(STDERR_FILENO, "cd: OLDPWD not set\n", 19);
			if (old_pwd)
				free(old_pwd);
			if (path)
				free(path);
			set_exit_status(shell, 1);
			return (1);
		}
		printf("%s\n", path); // Print the directory we're going to
		should_free_path = 1;
	}
	else
	{
		path = args[1];
		should_free_path = 0; // Don't free args[1], it's not our memory
	}
	
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

	// Only update PWD/OLDPWD if chdir succeeded
	update_pwd_vars(shell, old_pwd);
	
	if (old_pwd)
		free(old_pwd);
	if (should_free_path)
		free(path);
	set_exit_status(shell, 0);
	return (0);
}
