#include "minishell.h"

/* Helper function to get environment variable from shell's envp */
static char	*get_env_from_envp(char **envp, const char *name)
{
    int		i;
    int		name_len;

    if (!envp || !name)
        return (NULL);

    name_len = ft_strlen(name);
    i = 0;
    while (envp[i])
    {
        if (ft_strncmp(envp[i], name, name_len) == 0 && 
            envp[i][name_len] == '=')
            return (envp[i] + name_len + 1);
        i++;
    }
    return (NULL);
}

/* Path resolution */
char	*find_command_path(char *cmd, char **envp)
{
    char	**paths;
    char	*path_env;
    char	*full_path;
    char	*temp;
    int		i;

    if (!cmd)
        return (NULL);
    
    // If command contains '/' (absolute or relative path), check if it exists
    if (ft_strchr(cmd, '/'))
    {
        if (access(cmd, F_OK) == 0)
            return (ft_strdup(cmd));
        return (NULL);
    }
    
    // For commands without '/', search in PATH from shell's environment
    path_env = get_env_from_envp(envp, "PATH");
    if (!path_env)
        return (NULL);
    
    paths = ft_split(path_env, ':');
    if (!paths)
        return (NULL);
    
    i = 0;
    while (paths[i])
    {
        temp = ft_strjoin(paths[i], "/");
        if (temp)
        {
            full_path = ft_strjoin(temp, cmd);
            free(temp);
            if (full_path)
            {
                // Check if file exists AND is executable
                if (access(full_path, F_OK) == 0 && access(full_path, X_OK) == 0)
                {
                    free_str_array(paths);
                    return (full_path);
                }
                free(full_path);
            }
        }
        i++;
    }
    
    free_str_array(paths);
    return (NULL);
}

void	free_str_array(char **arr)
{
	int	i;

	if (!arr)
		return;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}
