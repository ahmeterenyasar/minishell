#ifndef SHELL_MAIN_H
# define SHELL_MAIN_H

# include "types.h"

/* Main shell loop functions */
void				run_shell_main_loop(t_shell_data *shell);

/* Input handling functions */
int					get_user_input(char **input, t_shell_data *shell);

/* Command processing functions */
int					process_single_command(char *line, t_shell_data *shell);
void				process_multiple_commands(char **lines,
						t_shell_data *shell);

/* Signal handling functions for main */
void				handle_signal_check(t_shell_data *shell);
void				handle_sigint_in_readline(t_shell_data *shell,
						char **input);
void				handle_sigint_after_readline(t_shell_data *shell,
						char *input);
void				handle_sigint_in_parsing(t_shell_data *shell);

/* Global signal variable */
extern sig_atomic_t	g_signal;

#endif
