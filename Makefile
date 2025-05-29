NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinclude
SRCS = main.c \
        src/parser/parser.c \
        src/parser/string_utils.c \
        src/parser/parser_interface.c \
        src/parser/syntax_checker.c \
        src/parser/utils.c \
        src/parser/utils_debug.c \
        src/parser/free.c \
        src/parser/tokenizer.c \
        src/parser/tokenization_utils.c \
        src/parser/tokenization_handler.c \
        src/parser/expansions.c \
        src/parser/heredoc.c \
        src/parser/signals.c \
        src/parser/white_space.c \
        src/parser/exit_status.c \
        src/parser/printf_utils.c \
		
OBJS = $(SRCS:.c=.o)
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(LIBFT) $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -lreadline
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

re: fclean all
	@echo "Recompiled successfully!"

.PHONY: all clean fclean re
