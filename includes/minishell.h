/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpinna-l <mpinna-l@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 16:38:01 by mpinna-l          #+#    #+#             */
/*   Updated: 2022/12/29 23:05:10 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define ECHO 1
# define ENV 2
# define PWD 3
# define EXIT 21

# include "libft/libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <signal.h>
# include <sys/wait.h>

char	**parse_command(char *statement, char **env);
void	command_executor(char *cmd_path, char **args, char **env);

// builtin
int		ft_echo(char **args);
int		ft_exit(char **args);
int		ft_env(char **env);
int		ft_pwd(char **args, char **env);

// error handling
int		print_err_msg(void);

// clone env
char	**build_env(char **env);

// signals handling
void	handle_signals(void);

// utils
int		is_builtin(char *cmd_path);
void	free2d(void **matrix2d);
#endif
