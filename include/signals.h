/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 12:00:12 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 12:00:13 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include "types.h"

/* Signal handling functions */
void			setup_signals(int context);
void			setup_signals_interactive(void);
void			setup_signals_executing(void);
void			setup_signals_heredoc(void);
void			reset_signals(void);

/* Signal handler functions */
void			handle_sigint_interactive(int signo);
void			handle_sigint_executing(int signo);
void			handle_sigquit_executing(int signo);
void			handle_sigint_heredoc(int signo);

/* Shell data management */
t_shell_data	*init_shell_data(char **envp);
void			set_exit_status(t_shell_data *shell, int status);
int				get_exit_status(t_shell_data *shell);
void			free_shell_data(t_shell_data *shell);
void			set_current_lines(t_shell_data *shell, char **lines);
void			clear_current_lines(t_shell_data *shell);

#endif