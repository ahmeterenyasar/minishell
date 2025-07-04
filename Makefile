NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinclude
SRCS = main.c \
        src/parser/string_utils.c \
        src/parser/parser_interface.c \
        src/parser/syntax_checker.c \
        src/parser/utils.c \
        src/parser/free.c \
        src/parser/tokenizer.c \
        src/parser/tokenizer_quoted_part.c \
        src/parser/tokenizer_unquoted_part.c \
        src/parser/tokenizer_boundary_check.c \
        src/parser/tokenizer_concatenate.c \
        src/parser/tokenizer_single_token.c \
        src/parser/tokenizer_finalize.c \
        src/parser/tokenization_utils.c \
        src/parser/tokenization_handler.c \
        src/parser/token_quote_utils.c \
        src/parser/input_processor.c \
        src/parser/token_word_utils.c \
        src/parser/expansions.c \
        src/parser/expansion_utils.c \
        src/parser/env_expansion.c \
        src/parser/concat_expansion_utils.c \
        src/parser/concat_expansion.c \
        src/parser/token_expansion.c \
        src/parser/redirect_expansion.c \
        src/parser/signals.c \
        src/parser/white_space.c \
        src/parser/shell_init.c \
        src/parser/shell_cleanup.c \
        src/parser/exit_status_manager.c \
        src/parser/current_lines_manager.c \
        src/parser/printf_utils.c \
        src/parser/arg_counter.c \
        src/parser/arg_filler.c \
        src/parser/command_parser.c \
        src/parser/redirection_parser.c \
        src/parser/redirection_utils.c \
        src/executor/builtin_cd.c \
        src/executor/builtin_cd_utils.c \
        src/executor/builtin_echo.c \
        src/executor/builtin_env.c \
        src/executor/builtin_executor.c \
        src/executor/builtin_exit.c \
        src/executor/builtin_expr.c \
        src/executor/builtin_export.c \
        src/executor/env_memory.c \
        src/executor/env_search.c \
        src/executor/env_variable_helpers.c \
        src/executor/env_operations.c \
        src/executor/env_interface.c \
        src/executor/builtin_export_display.c \
        src/executor/builtin_pwd.c \
        src/executor/builtin_unset.c \
        src/executor/builtin_utils.c \
        src/executor/executor.c \
        src/executor/child_cleanup.c \
        src/executor/command_validation.c \
        src/executor/process_signals.c \
        src/executor/builtin_execution.c \
        src/executor/child_execution.c \
        src/executor/executor_main.c \
        src/executor/path.c \
        src/executor/pipe_management.c \
        src/executor/pipeline_execution.c \
        src/executor/child_cleanup_utils.c \
        src/executor/child_command_utils.c \
        src/executor/child_external_execution.c \
        src/executor/pipeline_child_execution.c \
        src/executor/pipeline_coordinator.c \
        src/executor/pipe_line_utils.c \
        src/executor/redirection_handling.c \
        src/executor/redirection_handling_utils.c \
        src/executor/heredoc_parser.c \
        src/executor/heredoc_executor.c \
        src/executor/heredoc_executor_helpers.c \

OBJS = $(SRCS:.c=.o)
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(LIBFT) $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -lreadline $(LIBFT)
	@echo "minishell compiled successfully!"
	@echo "Run './$(NAME)' to start the shell."
	@echo "Type 'exit' to quit the shell."
	@echo "Have fun!"

clean:
	@rm -f $(OBJS)
	@$(MAKE) -C $(LIBFT_DIR) clean
	@echo "Object files cleaned."

fclean:
	@rm -f $(OBJS) $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@echo "All files cleaned."

re: fclean all clean
	@echo "Recompiled successfully!"

.PHONY: all clean fclean re