#ifndef EXPANSION_H
# define EXPANSION_H

# include "types.h"

/* Utility functions */
void	copy_env_value(char *result, int *j, char *value);
int		check_dollar_expansion(const char *str, int i);
char	*allocate_result_buffer(void);
void	copy_regular_char(char *result, int *j, char c);

/* Environment variable expansion */
int		extract_env_name(const char *str, int i, char *name, int max_len);
char	*expand_env_vars(const char *str, t_shell_data *shell);
void	expand_loop(const char *str, char *result, char *var_name,
			t_shell_data *shell);
char	*get_env_value(const char *name, t_shell_data *shell);
char	*get_exit_status_str(t_shell_data *shell);

/* Concatenated variable expansion */
void	handle_special_vars(const char *str, int *i, char *var_name,
			t_shell_data *shell, char *result, int *j);
void	handle_regular_vars(const char *str, int *i, char *var_name,
			t_shell_data *shell, char *result, int *j);
void	skip_leading_whitespace(const char *str, int *i, char *result, int *j);
char	*expand_concatenated_vars(const char *str, t_shell_data *shell);

/* Token expansion */
void	expand_tokens(t_token *tokens, t_shell_data *shell);

/* Redirect expansion */
void	expand_command_redirects(t_command *cmd, t_shell_data *shell);

#endif