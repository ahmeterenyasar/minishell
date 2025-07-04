#ifndef MINISHELL_H
# define MINISHELL_H

/* System includes */
# include "../libft/libft.h"
# include <ctype.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

/* Minishell module headers */
# include "builtins.h"
# include "executor.h"
# include "expansions.h"
# include "heredoc.h"
# include "parser.h"
# include "redirection.h"
# include "signals.h"
# include "string_utils.h"
# include "syntax_checker.h"
# include "tokenizer.h"
# include "types.h"

#endif