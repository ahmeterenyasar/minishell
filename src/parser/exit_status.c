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
    shell->current_lines = NULL;
    
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
    // Ensure status is within valid range (0-255)
    shell->last_exit_status = status & 0xFF;
}

int	get_exit_status(t_shell_data *shell)
{
    if (!shell)
        return (0);
    return (shell->last_exit_status);
}

/* Helper function to safely manage current_lines */
void	set_current_lines(t_shell_data *shell, char **lines)
{
    if (!shell)
        return ;
    
    // Free existing lines if any
    if (shell->current_lines)
    {
        free_str_array(shell->current_lines);
        shell->current_lines = NULL;
    }
    
    // Set new lines
    shell->current_lines = lines;
}

void	clear_current_lines(t_shell_data *shell)
{
    if (!shell)
        return ;
    
    if (shell->current_lines)
    {
        free_str_array(shell->current_lines);
        shell->current_lines = NULL;
    }
}

void	free_shell_data(t_shell_data *shell)
{
    if (!shell)
        return ;
    
    // Free current_lines if set (should only be called from main shell, not child)
    if (shell->current_lines)
    {
        free_str_array(shell->current_lines);
        shell->current_lines = NULL;
    }
    
    // Free environment variables (heap copies we created)
    if (shell->envp)
    {
        free_envp(shell->envp);
        shell->envp = NULL;
    }
    
    // Free the shell structure itself
    free(shell);
}
