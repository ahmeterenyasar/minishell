#ifndef SIGNALS_H
# define SIGNALS_H

# include "types.h"

/* Signal handling functions */
void						setup_signals(int context);
void						reset_signals(void);

/* Shell data management */
t_shell_data				*init_shell_data(char **envp);
void						set_exit_status(t_shell_data *shell, int status);
int							get_exit_status(t_shell_data *shell);
void						free_shell_data(t_shell_data *shell);

#endif