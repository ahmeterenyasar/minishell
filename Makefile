
NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinclude
SRCS = main.c \
		src/parser/parser.c \

OBJS = $(SRCS:.c=.o)
LIBFT = libft/libft.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) -o $@ $^
	@echo "Minishell compiled successfully!"
	@echo "Run './$(NAME)' to start the shell."
	@echo "Type 'exit' to quit the shell."
	@echo "Have fun!"

clean:
	@rm -f $(OBJS)
	@echo "Object files cleaned."

fclean:
	@rm -f $(OBJS) $(NAME)
	@echo "All files cleaned."

re: fclean all
	@echo "Recompiled successfully!"

.PHONY: all clean fclean re
