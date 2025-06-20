#include "minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "expr") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

int	execute_builtin(char **args, t_shell_data *shell)
{
	if (!args || !args[0])
		return (1);
	if (ft_strcmp(args[0], "echo") == 0)
		return (execute_echo(args, shell));
	if (ft_strcmp(args[0], "cd") == 0)
		return (execute_cd(args, shell));
	if (ft_strcmp(args[0], "pwd") == 0)
		return (execute_pwd(shell));
	if (ft_strcmp(args[0], "export") == 0)
		return (execute_export(args, shell));
	if (ft_strcmp(args[0], "unset") == 0)
		return (execute_unset(args, shell));
	if (ft_strcmp(args[0], "env") == 0)
		return (execute_env(args, shell));
	if (ft_strcmp(args[0], "expr") == 0)
		return (execute_expr(args, shell));
	if (ft_strcmp(args[0], "exit") == 0)
		return (execute_exit(args, shell));
	return (1);
}
