# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/27 14:14:19 by lfarias-          #+#    #+#              #
#    Updated: 2023/01/15 16:02:48 by lfarias-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	minishell

CC			= 	cc	

CFLAGS		=	-Wall -Werror -Wextra -g -fsanitize=address 

LDLIBS		= 	-lreadline includes/libft.a

CPPFLAGS 	=   -I /Users/lfarias-/.brew/Cellar/readline/8.2.1/include

LDFLAGS 	=   -L  /Users/lfarias-/.brew/Cellar/readline/8.2.1/lib

SRC			= 	main.c command_executor.c command_pathfinder.c command_builder.c error_handler.c \
				signal_handlers.c echo.c exit.c build_env.c env.c pwd.c cd.c \
				export.c expander.c expander_utils.c unset.c quote_resolver.c \
				lexer.c parser.c parser_rules.c interpreter.c interpreter_utils.c \
				cleaner.c pipes.c redirections.c redirections2.c here_doc.c error.c

SRCS		= 	$(addprefix src/,$(SRC))

OBJS		= 	$(SRCS:.c=.o)

INCLUDES	=	-I ./includes

%.o: %.c
	@${CC} -c $(CFLAGS) $(INCLUDES) ${CPPFLAGS} $< -o $@

${NAME}:   ${OBJS}
	@make -C ./includes/libft bonus
	@${CC} ${INCLUDES} ${CFLAGS} ${OBJS} -o ${NAME} ${LDLIBS} ${CPPFLAGS} ${LDFLAGS} 

all:    ${NAME}

clean:
	@make -C ./includes/libft clean
	@rm -f $(OBJS)

fclean: clean
	@make -C ./includes/libft fclean
	@rm -f $(NAME)

re:	fclean all

.PHONY: all name clean fclean re
