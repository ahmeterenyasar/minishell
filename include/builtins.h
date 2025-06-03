#ifndef BUILTINS_H
# define BUILTINS_H

# include "types.h"

/* Built-in command functions */
int							execute_echo(char **args, t_shell_data *shell);
int							execute_pwd(t_shell_data *shell);
int							execute_cd(char **args, t_shell_data *shell);
int							execute_env(char **args, t_shell_data *shell);
int							execute_export(char **args, t_shell_data *shell);
int							set_env_variable(t_shell_data *shell, const char *name, const char *value);
int							find_env_var(char **envp, const char *name);
int							execute_unset(char **args, t_shell_data *shell);
int							execute_exit(char **args, t_shell_data *shell);
int							is_builtin(char *cmd);
int							execute_builtin(char **args, t_shell_data *shell);

/* Helper functions for builtins */
int							is_valid_n_flag(const char *arg);
int							is_valid_identifier(const char *name);
int							is_numeric_string(const char *str);
void						update_pwd_vars(t_shell_data *shell, char *old_pwd);
void						print_export_vars(char **envp);
void						print_env(char **envp);
void						free_envp(char **envp);
char						**copy_envp(char **envp);

#endif