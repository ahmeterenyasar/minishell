#include "minishell.h"

static void	process_dollar_sign(const char *str, int *i, int *j, 
	char *result, char *var_name, t_shell_data *shell)
{
	char	*value;

	(*i)++;
	*i = extract_env_name(str, *i, var_name, 256);
	value = get_env_value(var_name, shell);
	copy_env_value(result, j, value);
}

void	expand_loop(const char *str, char *result, 
	char *var_name, t_shell_data *shell)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i] && j < 4095)
	{
		if (check_dollar_expansion(str, i))
		{
			process_dollar_sign(str, &i, &j, result, var_name, shell);
		}
		else
		{
			copy_regular_char(result, &j, str[i]);
			i++;
		}
	}
	result[j] = '\0';
}

char	*expand_env_vars(const char *str, t_shell_data *shell)
{
	char	*result;
	char	var_name[256];

	if (!str)
		return (ft_strdup(""));
	result = allocate_result_buffer();
	if (!result)
		return (ft_strdup(""));
	expand_loop(str, result, var_name, shell);
	return (result);
}
