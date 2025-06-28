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
            // Use the improved expansion function that handles concatenated vars correctly
            expanded = expand_concatenated_vars(current->value, shell);
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

/* Advanced expansion function that handles concatenated tokens correctly */
char	*expand_concatenated_vars(const char *str, t_shell_data *shell)
{
	char	*result;
	int		i, j;
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
			// Found a variable - extract and expand it
			i++; // Skip the $
			
			// Extract variable name with proper boundary checking
			if (str[i] == '?' || str[i] == '$')
			{
				// Special single-character variables
				var_name[0] = str[i];
				var_name[1] = '\0';
				i++;
			}
			else
			{
				// Regular variable name - use longest match that exists
				int name_len = 0;
				int best_len = 0;
				char *best_value = NULL;
				
				// Try progressively longer variable names to find the longest match
				while (str[i + name_len] && (ft_isalnum(str[i + name_len]) || str[i + name_len] == '_') && name_len < 255)
				{
					name_len++;
					// Create variable name of this length
					ft_strncpy(var_name, str + i, name_len);
					var_name[name_len] = '\0';
					
					// Check if this variable exists
					value = get_env_value(var_name, shell);
					if (value && *value)
					{
						// Found a match - remember it
						if (best_value)
							free(best_value);
						best_value = value;
						best_len = name_len;
					}
					else if (value)
						free(value);
				}
				
				// Use the longest match found, or the full name if no match
				if (best_len > 0)
				{
					i += best_len;
					if (best_value && *best_value)
					{
						int k = 0;
						while (best_value[k] && j < 4095)
							result[j++] = best_value[k++];
					}
					if (best_value)
						free(best_value);
				}
				else
				{
					// No match found - copy the $ and continue
					result[j++] = '$';
				}
			}
		}
		else
		{
			// Regular character - copy as-is
			result[j++] = str[i++];
		}
	}
	result[j] = '\0';
	return (result);
}
