#include "minishell.h"

void	copy_env_value(char *result, int *j, char *value)
{
    int	k;

    if (!value || !*value)
    {
        if (value)
            free(value);
        return;
    }
    k = 0;
    while (value[k] && *j < 4095)
    {
        result[(*j)++] = value[k++];
    }
    free(value);
}

static char	*expand_env_vars_in_redirects(const char *str, t_shell_data *shell)
{
    char	*result;
    int		i;
    int		j;
    char	var_name[256];
    char	*value;

    if (!str)
        return (ft_strdup(""));
    result = malloc(4096);
    if (!result)
        return (ft_strdup(""));
    i = 0;
    j = 0;
    while (str[i] && j < 4095)
    {
        if (str[i] == '$' && str[i + 1] && (ft_isalnum(str[i + 1]) 
            || str[i + 1] == '_' || str[i + 1] == '?' || str[i + 1] == '$'))
        {
            i++;
            i = extract_env_name(str, i, var_name, sizeof(var_name));
            value = get_env_value(var_name, shell);
            copy_env_value(result, &j, value);
        }
        else
            result[j++] = str[i++];
    }
    result[j] = '\0';
    return (result);
}

char	*expand_env_vars(const char *str, t_shell_data *shell)
{
    char	*result;
    int		i;
    int		j;
    char	var_name[256];
    char	*value;

    if (!str)
        return (ft_strdup(""));
    result = malloc(4096);
    if (!result)
        return (ft_strdup(""));
    i = 0;
    j = 0;
    while (str[i] && j < 4095)
    {
        if (str[i] == '$' && str[i + 1] && (ft_isalnum(str[i + 1]) 
            || str[i + 1] == '_' || str[i + 1] == '?' || str[i + 1] == '$'))
        {
            i++;
            i = extract_env_name(str, i, var_name, sizeof(var_name));
            value = get_env_value(var_name, shell);
            copy_env_value(result, &j, value);
        }
        else
            result[j++] = str[i++];
    }
    result[j] = '\0';
    return (result);
}

static void	expand_redirect_files(t_redirect *redirects, t_shell_data *shell)
{
    t_redirect	*current;
    char		*expanded;

    current = redirects;
    while (current)
    {
        if (current->file && current->type != TOKEN_HEREDOC)
        {
            expanded = expand_env_vars_in_redirects(current->file, shell);
            if (expanded)
            {
                free(current->file);
                current->file = expanded;
            }
        }
        current = current->next;
    }
}

void	expand_tokens(t_token *tokens, t_shell_data *shell)
{
    t_token *current;
    char *expanded;

    current = tokens;
    while (current)
    {
        if (current->type == TOKEN_WORD && current->expandable)
        {
            expanded = expand_env_vars(current->value, shell);
            if (expanded)
            {
                free(current->value);
                current->value = expanded;
            }
        }
        current = current->next;
    }
}

void	expand_command_redirects(t_command *cmd, t_shell_data *shell)
{
    t_command	*current;

    current = cmd;
    while (current)
    {
        if (current->redirects)
            expand_redirect_files(current->redirects, shell);
        current = current->next;
    }
}
