NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinclude
SRCS = main.c \
		src/parser/parser.c \
		src/parser/lexer.c

OBJS = $(SRCS:.c=.o)
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(LIBFT) $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) -o $@ $^
	@echo "Minishell compiled successfully!"
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
