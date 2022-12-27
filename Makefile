NAME= minishell

FLAGS= -Wall -Werror -Wextra

SRC= main.c

SRCS= $(addprefix src/,$(SRC))

OBJS= $(SRCS:.c=.o)

INCLUDES= -lreadline -I ./includes

all:    NAME

NAME:   ${OBJS}
	@cc -o ${NAME} ${INCLUDES} ${OBJS} ${FLAGS}

%.o: %.c
	@cc -c $(FLAGS) $(INCLUDES) $< -o $@

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re:	fclean all

.PHONY: all name clean fclean re
