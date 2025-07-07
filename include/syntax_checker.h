/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 12:00:17 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 12:00:18 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYNTAX_CHECKER_H
# define SYNTAX_CHECKER_H

# include "types.h"

/* Main syntax validation */
int		check_syntax(t_token *tokens);

/* Utility functions */
int		is_redirection(t_token_type type);
t_token	*skip_empty_tokens(t_token *token);

/* Specific syntax checkers */
int		check_start_syntax(t_token *tokens);
int		check_redirection_syntax(t_token *tokens);
int		check_pipe_syntax(t_token *tokens);
int		check_consecutive_redirections(t_token *tokens);

/* Error handling */
void	print_redirection_error(t_token_type type);

#endif
