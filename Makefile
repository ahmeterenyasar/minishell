NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinclude
SRCS = main.c \
        src/parser/string_utils.c \
        src/parser/parser_interface.c \
        src/parser/syntax_checker.c \
        src/parser/utils.c \
        src/parser/utils_debug.c \
        src/parser/free.c \
        src/parser/tokenizer.c \
        src/parser/tokenization_utils.c \
        src/parser/tokenization_handler.c \
        src/parser/token_quote_utils.c \
        src/parser/input_processor.c \
        src/parser/token_word_utils.c \
        src/parser/expansions.c \
        src/parser/expansion_of_tokens.c \
        src/parser/signals.c \
        src/parser/white_space.c \
        src/parser/exit_status.c \
        src/parser/printf_utils.c \
        src/parser/arg_counter.c \
        src/parser/arg_filler.c \
        src/parser/command_parser.c \
        src/parser/redirection_parser.c \
        src/parser/redirection_utils.c \
        src/executor/builtin_cd.c \
        src/executor/builtin_echo.c \
        src/executor/builtin_env.c \
        src/executor/builtin_executor.c \
        src/executor/builtin_exit.c \
        src/executor/builtin_export.c \
        src/executor/builtin_export_utils.c \
        src/executor/builtin_export_utils_2.c \
        src/executor/builtin_pwd.c \
        src/executor/builtin_unset.c \
        src/executor/builtin_utils.c \
        src/executor/executor.c \
        src/executor/path.c \
        src/executor/pipe_lines.c \
        src/executor/redirection_handling.c \
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