#include "minishell.h"

int	execute_cd(char **args, t_shell_data *shell)
{
	char	*path;
	char	*old_pwd;
	int		should_free_path;

	if (validate_cd_args(args, shell))
		return (1);
	old_pwd = getcwd(NULL, 0);
	should_free_path = 0;
	if (determine_cd_path(args, shell, old_pwd, &path, &should_free_path))
		return (1);
	return (change_to_directory(path, old_pwd, should_free_path, shell));
}
