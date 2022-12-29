# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/27 14:14:19 by lfarias-          #+#    #+#              #
#    Updated: 2022/12/29 19:49:13 by mpinna-l         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	minishell

CC			= 	cc

CFLAGS		=	-Wall -Werror -Wextra

LDLIBS		= 	-lreadline includes/libft.a

SRC			= 	main.c command_executor.c command_loader.c error_handler.c signal_handlers.c echo.c build_env.c env.c

SRCS		= 	$(addprefix src/,$(SRC))

OBJS		= 	$(SRCS:.c=.o)

INCLUDES	=	-I ./includes

%.o: %.c
	@${CC} -c $(CFLAGS) $(INCLUDES) $< -o $@

${NAME}:   ${OBJS}
	@make -C ./includes/libft
	@${CC} -o ${NAME} ${INCLUDES} ${OBJS} ${CFLAGS} ${LDLIBS}

all:    ${NAME}
clean:
	@make -C ./includes/libft clean
	@rm -f $(OBJS)

fclean: clean
	@make -C ./includes/libft fclean
	@rm -f $(NAME)

re:	fclean all

.PHONY: all name clean fclean re
