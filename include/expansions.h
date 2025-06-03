#ifndef EXPANSION_H
# define EXPANSION_H

# include "types.h"

/* Expansion functions */
int							extract_env_name(const char *str, int i, char *name, int max_len);
void						copy_env_value(char *result, int *j, char *value);

/* Environment variable functions */
char						*expand_env_vars(const char *str, t_shell_data *shell);
char						*get_env_value(const char *name, t_shell_data *shell);
void						expand_tokens(t_token *tokens, t_shell_data *shell);
char						*get_exit_status_str(t_shell_data *shell);
void						expand_command_redirects(t_command *cmd, t_shell_data *shell);

#endif