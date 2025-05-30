#include "minishell.h"

char	*get_exit_status_str(t_shell_data *shell)
{
    int		status;

    status = get_exit_status(shell);
    return (int_to_string(status));
}

char	*get_shell_pid(void)
{
    return (int_to_string(getpid()));
}

char	*get_env_value(const char *name, t_shell_data *shell)
{
    int		i;
    int		len;
    char	**envp;

    if (!name || !shell || !shell->envp)
        return (ft_strdup(""));  // Return empty string, not NULL
    if (strcmp(name, "?") == 0)
        return (get_exit_status_str(shell));
    if (strcmp(name, "$") == 0)
        return (get_shell_pid());
    len = strlen(name);
    envp = shell->envp;
    i = 0;
    while (envp[i])
    {
        if (strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
            return (strdup(envp[i] + len + 1));
        i++;
    }
    // Variable not found - return empty string (bash behavior)
    return (ft_strdup(""));
}

int	extract_env_name(const char *str, int i, char *name, int max_len)
{
    int	j;

    j = 0;
    if (str[i] == '?' || str[i] == '$')
    {
        name[j++] = str[i++];
        name[j] = '\0';
        return (i);
    }
    while (str[i] && (ft_isalnum(str[i]) || str[i] == '_') && j < max_len - 1)
    {
        name[j++] = str[i++];
    }
    name[j] = '\0';
    return (i);
}
