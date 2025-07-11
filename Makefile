NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinclude
SRCS = main.c \
        src/tokenizer/tokenizer.c \
        src/tokenizer/tokenizer_quoted_part.c \
        src/tokenizer/tokenizer_unquoted_part.c \
        src/tokenizer/tokenizer_word_part_utils.c \
        src/tokenizer/tokenizer_boundary_check.c \
        src/tokenizer/tokenizer_concatenate.c \
        src/tokenizer/tokenizer_single_token.c \
        src/tokenizer/tokenizer_finalize.c \
        src/tokenizer/token_creation.c \
        src/tokenizer/token_list.c \
        src/tokenizer/token_processing.c \
        src/tokenizer/token_utils.c \
        src/tokenizer/tokenization_handler.c \
        src/tokenizer/token_quote_utils.c \
        src/tokenizer/token_word_utils.c \
        src/tokenizer/input_processor.c \
        src/tokenizer/input_processor_utils.c \
        src/tokenizer/white_space.c \
        src/parser/parser_interface.c \
        src/parser/syntax_validator.c \
        src/parser/syntax_checker_utils.c \
        src/parser/syntax_checker_redirection.c \
        src/parser/syntax_checker_pipe.c \
        src/parser/command_parser.c \
        src/parser/redirection_parser.c \
        src/parser/redirection_utils.c \
        src/parser/arg_counter.c \
        src/parser/arg_filler.c \
        src/parser/utils.c \
        src/parser/free.c \
        src/expansion/expansions.c \
        src/expansion/expansion_utils.c \
        src/expansion/env_expansion.c \
        src/expansion/concat_expansion_utils.c \
        src/expansion/concat_loop_utils.c \
        src/expansion/concat_expansion.c \
        src/expansion/token_expansion.c \
        src/expansion/redirect_expansion.c \
        src/executor/executor.c \
        src/executor/executor_main.c \
        src/executor/command_processor.c \
        src/executor/command_validation.c \
        src/executor/child_execution.c \
        src/executor/child_cleanup.c \
        src/executor/child_cleanup_utils.c \
        src/executor/child_command_utils.c \
        src/executor/execve_cleanup.c \
        src/executor/execve_error_handling.c \
        src/executor/execve_args_utils.c \
        src/executor/execve_executor.c \
        src/executor/path_resolution.c \
        src/executor/environment_utils.c \
        src/builtins/builtin_cd.c \
        src/builtins/builtin_cd_utils.c \
        src/builtins/builtin_echo.c \
        src/builtins/builtin_env.c \
        src/builtins/builtin_execution.c \
        src/builtins/builtin_executor.c \
        src/builtins/builtin_exit.c \
        src/builtins/builtin_exit_utils.c \
        src/builtins/builtin_expr.c \
        src/builtins/builtin_expr_validation.c \
        src/builtins/builtin_export.c \
        src/builtins/builtin_export_validation.c \
        src/builtins/builtin_export_error_handling.c \
        src/builtins/builtin_export_validation_utils.c \
        src/builtins/builtin_export_assignment_processing.c \
        src/builtins/builtin_export_display.c \
        src/builtins/builtin_export_sorting.c \
        src/builtins/builtin_pwd.c \
        src/builtins/builtin_unset.c \
        src/builtins/builtin_utils.c \
        src/environment/env_memory.c \
        src/environment/env_search.c \
        src/environment/env_variable_helpers.c \
        src/environment/env_add_operations.c \
        src/environment/env_remove_operations.c \
        src/environment/env_unset_operations.c \
        src/environment/env_interface.c \
        src/pipeline/pipe_management.c \
        src/pipeline/pipeline_execution.c \
        src/pipeline/pipeline_child_execution.c \
        src/pipeline/pipeline_coordinator.c \
        src/pipeline/pipe_line_utils.c \
        src/redirection/redirection_input.c \
        src/redirection/redirection_output.c \
        src/redirection/redirection_validation.c \
        src/redirection/redirection_coordinator.c \
        src/redirection/redirection_file_utils.c \
        src/redirection/heredoc_parser.c \
        src/redirection/heredoc_executor.c \
        src/redirection/heredoc_executor_helpers.c \
        src/redirection/heredoc_core_utils.c \
        src/redirection/heredoc_line_processing.c \
        src/redirection/heredoc_reading.c \
        src/redirection/heredoc_setup.c \
        src/redirection/heredoc_file_utils.c \
        src/signals/signals.c \
        src/signals/signal_handlers.c \
        src/signals/signal_management.c \
        src/signals/process_signals.c \
        src/signals/shell_signal_handlers.c \
        src/core/shell_init.c \
        src/core/shell_cleanup.c \
        src/core/shell_main_loop.c \
        src/core/shell_input_handler.c \
        src/core/multiline_input_utils.c \
        src/core/exit_status_manager.c \
        src/core/current_lines_manager.c \
        src/core/print_utils.c

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