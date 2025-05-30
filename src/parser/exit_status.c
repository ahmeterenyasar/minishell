#include "minishell.h"

t_shell_data	*init_shell_data(char **envp)
{
    t_shell_data	*shell;
    int				count;
    int				i;

    shell = malloc(sizeof(t_shell_data));
    if (!shell)
        return (NULL);
    shell->last_exit_status = 0;
    
    // Count environment variables
    count = 0;
    while (envp[count])
        count++;

    // Allocate space for environment array
    shell->envp = malloc(sizeof(char *) * (count + 1));
    if (!shell->envp)
    {
        free(shell);
        return (NULL);
    }

    // Create heap copies of each environment variable
    i = 0;
    while (i < count)
    {
        shell->envp[i] = ft_strdup(envp[i]);
        if (!shell->envp[i])
        {
            // Cleanup on failure
            while (--i >= 0)
                free(shell->envp[i]);
            free(shell->envp);
            free(shell);
            return (NULL);
        }
        i++;
    }
    shell->envp[count] = NULL;
    
    return (shell);
}

void	set_exit_status(t_shell_data *shell, int status)
{
    if (!shell)
        return ;
    shell->last_exit_status = status;
}

int	get_exit_status(t_shell_data *shell)
{
    if (!shell)
        return (0);
    return (shell->last_exit_status);
}

void	free_shell_data(t_shell_data *shell)
{
    if (!shell)
        return ;
    
    // Now we can safely free envp since we created heap copies
    free_envp(shell->envp);
    free(shell);
}
