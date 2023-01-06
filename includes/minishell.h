/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpinna-l <mpinna-l@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 16:38:01 by mpinna-l          #+#    #+#             */
/*   Updated: 2023/01/06 15:05:04 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define ECHO 1
# define ENV 2
# define PWD 3
# define CD 6
# define EXIT 21
# define EXPORT 4
# define UNSET 5

// SYNTAX tokens defines
# define WORD 42
# define PIPE 64 
# define REDIRECT 128

# include "libft/libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <signal.h>
# include <sys/wait.h>

typedef struct s_env
{
	char **env;
} t_env;

/* According to the bash grammar
 *
 * <, <<, >, >>, | and $ are special characters. except $ all are considered operators
 */

typedef struct s_lexeme
{
	int		type;
	char	*value;
}	t_token;

/* A command is composed of multiple tokens of different types and values
*	this is what we will give to our executor 
*/

typedef struct s_expression
{
	t_token **tokens;	
	int		has_pipe;
}	t_command;

typedef struct s_kludge
{
    char    *str;
    char    *variable;
    int     j;
    char    *temp;
    int     flag;

}   t_kludge;

char	*parse_command(char *statement, char **env);
void	command_executor(char *cmd_path, char **args, t_env *env);

// builtin
int		ft_echo(char **args);
int		ft_exit(char **args);
int		ft_env(char **env);
int		ft_pwd(char **args, char **env);
int		ft_cd(char **args, char **env);
int		ft_export(char **args, t_env *env);
int		ft_unset(char **args, t_env *env);

// Parser and Lexer
t_list		*make_tokens(char *user_input);
t_command	*parse_expression(t_list **token_list);
int			check_syntax(t_list *token_list);

int			pipe_rules(t_token *prev_tkn, t_token *curr_token, t_token *next_tkn);
int			redirect_rules(t_token *curr_token, t_token *next_tkn);

// interpreter
void	eval_tokens(t_list **tokens, t_env *env_clone);

// error handling
int		print_err_msg(void);

// clone env
char	**build_env(char **env);

// signals handling
void	handle_signals(void);

// quotes
char	*quote_resolver(char *str);
int		update_quote(char *str, int i, int *quote_flag);

// Expand variables
char    *expanded_str(char *input, char **env);
char    *env_search(char *variable, char **env, int *j);

//kludge
void    init_kludge(t_kludge *kludge, char *input);
void    expand_variable(t_kludge *kludge, char **env, char *input, int i);
void    copy_variable(t_kludge *kludge);
char    *env_search(char *variable, char **env, int *j);

// utils
int		is_builtin(char *cmd_path);
void	free2d(void **matrix2d);

#endif
