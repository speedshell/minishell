/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpinna-l <mpinna-l@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 16:38:01 by mpinna-l          #+#    #+#             */
/*   Updated: 2023/01/13 13:43:23 by lfarias-         ###   ########.fr       */
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
# define HERE_DOC 256

# include "libft/libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <signal.h>
# include <sys/wait.h>

typedef struct s_env
{
	char	**env;
}	t_env;

/* According to the bash grammar
 *
 * <, <<, >, >>, | and $ are special characters. 
 * except $, all of the above are considered operators
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
	t_token	**tokens;	
	int		has_pipe;
	int		has_redirect;
	int		redirect[2];
	int		in_pipe[2];
	int		out_pipe[2];
	int		return_code;
}	t_command;

char		*command_find_path(char *statement, char **env);
void		command_executor(char **cmd_path, t_command *expr, t_env *env);

// builtin
int			ft_echo(char **args);
int			ft_exit(char **args);
int			ft_env(char **env);
int			ft_pwd(char **args, char **env);
int			ft_cd(char **args, char **env);
int			ft_export(char **args, t_env *env);
int			ft_unset(char **args, t_env *env);

// Parser and Lexer
t_list		*make_tokens(char *user_input);
t_command	*parse_expression(t_list **token_list);
int			check_syntax(t_list *token_list);

int			pipe_rules(t_token *p_tkn, t_token *curr_token, t_token *nxt_tkn);
int			redirect_rules(t_token *curr_token, t_token *next_tkn);

// interpreter
int			eval_tokens(t_list **tokens, t_env *env_clone);
t_list		*init_vars(t_command **expr, char ***cmd, int *pp, t_list **tks);
int			init_pipe(t_command *expr);
void		copy_pipes_fds(int *dest, int *src);
char		*args_eval(char *arg, char **env);
int			alloc_fields(t_command *expr, int *field_count, char ***cmd);

// error handling
int			print_err_msg(void);

// clone env
char		**build_env(char **env);

// signals handling
void		handle_signals(void);

// pipes
void		pipes_setup(t_command *expr);
void		pipes_close(t_command *expr);
void		pipes_builtin_setup(t_command *expr, int *std_backup);
void		pipes_builtin_close(t_command *expr, int *std_backup);

// redirection
int			file_open_read(char *filename, int *redirect);
int			file_open_write(char *filename, int *redirect, int mode);
int			here_doc(char *delimiter, int *redirect);
void		redirection_builtin_setup(t_command *expr, int *std_backup);
void		redirection_builtin_close(t_command *expr, int *std_backup);

// quotes
char		*quote_resolver(char *str);
int			update_quote(char *str, int i, int *quote_flag);

// Expand variables
int			valid_variable(char *c);
char		*expand_str(char *input, char **env);
char		*str_nodes_join(t_list *str_nodes);

// utils
int			is_builtin(char *cmd_path);

// cleaner
void		free2d(void **matrix2d);
void		free_token(void *tk);
#endif
