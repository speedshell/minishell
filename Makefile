# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/27 14:14:19 by lfarias-          #+#    #+#              #
#    Updated: 2022/12/27 15:05:35 by lfarias-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	minishell

CC			= 	cc

CFLAGS		=	-Wall -Werror -Wextra -g

LDLIBS		= 	-lreadline

SRC			= 	main.c

SRCS		= 	$(addprefix src/,$(SRC))

OBJS		= 	$(SRCS:.c=.o)

INCLUDES	=	-I ./includes

%.o: %.c
	@${CC} -c $(CFLAGS) $(INCLUDES) $< -o $@

${NAME}:   ${OBJS}
	@${CC} -o ${NAME} ${INCLUDES} ${OBJS} ${CFLAGS} ${LDLIBS}

all:    ${NAME}

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re:	fclean all

.PHONY: all name clean fclean re
