#ifndef TYPES_H
# define TYPES_H

# include <sys/types.h>
# include <signal.h>

/* Error Codes */
# define SUCCESS 0
# define SYNTAX_ERROR 1
# define COMMAND_NOT_FOUND 127

/* Signal handler context */
# define INTERACTIVE_MODE 0
# define EXECUTING_MODE 1
# define HEREDOC_MODE 2

/* Token Types */
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC
}							t_token_type;

/* Token Structure */
typedef struct s_token
{
	t_token_type			type;
	char					*value;
	int						expandable;
	struct s_token			*next;
}							t_token;

/* Redirection Structure */
typedef struct s_redirect
{
	t_token_type			type;
	char					*file;
	int						expand_heredoc;
	struct s_redirect		*next;
}							t_redirect;

/* Command Structure */
typedef struct s_command
{
	char					**args;
	t_redirect				*redirects;
	struct s_command		*next;
}							t_command;

/* Shell data structure to hold exit status and other shell state */
typedef struct s_shell_data
{
	int						last_exit_status;
	char					**envp;
}							t_shell_data;

/* Global variable for signal handling - ONLY allowed global */
extern sig_atomic_t			g_signal;

#endif