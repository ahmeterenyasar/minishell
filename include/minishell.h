#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <ctype.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <limits.h>
# include <stdbool.h>
#include <sys/ioctl.h>
#include <termios.h>

# include "../libft/libft.h"

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
    TOKEN_REDIR_IN,    // <
    TOKEN_REDIR_OUT,   // >
    TOKEN_APPEND,      // >>
    TOKEN_HEREDOC      // <<
}   t_token_type;

/* Token Structure */
typedef struct s_token
{
    t_token_type    type;
    char            *value;
    int             expandable;    // 1 if token can be expanded (not in single quotes)
    struct s_token  *next;
}   t_token;

/* Redirection Structure */
typedef struct s_redirect
{
    t_token_type    type;    // Redirection type
    char            *file;   // Filename or heredoc delimiter
    int             expand_heredoc;  // 1 if heredoc content should expand variables
    struct s_redirect *next;
}   t_redirect;

/* Command Structure */
typedef struct s_command
{
    char            **args;  // Command and arguments
    t_redirect      *redirects; // List of redirections
    struct s_command *next;  // Next command in pipeline
}   t_command;

/* Global variable for signal handling */
extern sig_atomic_t g_signal;

#endif