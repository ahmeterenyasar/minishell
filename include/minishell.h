#ifndef MINISHELL_H
# define MINISHELL_H

/* System includes */
# include "../libft/libft.h"
# include <sys/stat.h>
# include <ctype.h>
# include <errno.h>
# include <fcntl.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <readline/history.h>
# include <readline/readline.h>

/* Minishell module headers */
# include "types.h"
# include "string_utils.h"
# include "tokenizer.h"
# include "expansions.h"
# include "parser.h"
# include "signals.h"
# include "builtins.h"
# include "executor.h"
# include "redirection.h"
# include "heredoc.h"
# include "debug.h"

#endif