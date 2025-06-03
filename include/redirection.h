#ifndef REDIRECTION_H
# define REDIRECTION_H

# include "types.h"

/* Redirection handling functions */
int							setup_input_redirection(t_redirect *redirect);
int							setup_output_redirection(t_redirect *redirect);
int							setup_redirections(t_redirect *redirects);
int							check_file_permissions(const char *filename, int flags);
void						print_file_error(const char *filename, const char *operation);

#endif