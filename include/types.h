/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 12:00:23 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 14:31:51 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H

# include <signal.h>
# include <sys/types.h>

# define SUCCESS 0
# define SYNTAX_ERROR 1
# define COMMAND_NOT_FOUND 127

# define INTERACTIVE_MODE 0
# define EXECUTING_MODE 1
# define HEREDOC_MODE 2

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC
}						t_token_type;

typedef struct s_token
{
	t_token_type		type;
	char				*value;
	int					expandable;
	int					quoted;
	struct s_token		*next;
}						t_token;

typedef struct s_redirect
{
	t_token_type		type;
	char				*file;
	int					expand_heredoc;
	int					was_quoted;
	struct s_redirect	*next;
}						t_redirect;

typedef struct s_command
{
	char				**args;
	t_redirect			*redirects;
	struct s_command	*next;
}						t_command;

typedef struct s_quote_pos
{
	int					start;
	int					end;
	char				quote_char;
}						t_quote_pos;

typedef struct s_finalize_params
{
	int					start_pos;
	int					expandable;
	const char			*input;
	int					current_pos;
}						t_finalize_params;

typedef struct s_word_parts_params
{
	char				**word_parts;
	int					*part_count;
	int					*expandable;
}						t_word_parts_params;

typedef struct s_shell_data
{
	int					last_exit_status;
	char				**envp;
	char				**current_lines;
	int					should_exit;
}						t_shell_data;

typedef struct s_heredoc_context
{
	int					original_stdin;
	char				**clean_delimiter;
	int					expand;
	t_shell_data		*shell;
}						t_heredoc_context;

extern sig_atomic_t		g_signal;

#endif