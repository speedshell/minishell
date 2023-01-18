/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpinna-l <mpinna-l@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 16:38:01 by mpinna-l          #+#    #+#             */
/*   Updated: 2023/01/18 17:02:16 by lfarias-         ###   ########.fr       */
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
# include <errno.h>

/* highlander global 
*	this is just the definition 
*/
extern int	g_exit_code;

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

typedef struct s_pid_list
{
	int					pid;
	struct s_pid_list	*next;
} t_pid_l;

typedef struct s_expression
{
	t_token	**tokens;	
	int		builtin;
	int		has_pipe;
	int		pipe_chain;
	int		has_redirect;
	int		redirect[2];
	int		in_pipe[2];
	int		out_pipe[2];
	int		return_code;
}	t_command;

typedef struct s_shell_info
{
	char		**env;
	char		*read_line_buffer;
	t_list		*token_list;
	t_command	*expr;
	char		**cmd;
	t_list		*tmp_files;
	t_pid_l		*child_pids;
}	t_info;


char		**command_builder(t_info *shell_data);
char		*command_find_path(char *statement, char **env, int *builtin);
void		command_executor(t_info *shell_data);

// builtin
int			ft_echo(char **args);
int			ft_exit(t_info *shell_data);
int			ft_env(char **args, char **env);
int			ft_pwd(char **args, char **env);
int			ft_cd(char **args, char **env);
int			ft_export(t_info *shell_data);
int			ft_unset(t_info *shell_data);

// Parser and Lexer
t_list		*make_tokens(char *user_input);
t_command	*parse_expression(t_list **token_list);
int			check_syntax(t_list *token_list, t_info *shell_data);
int			pipe_rules(t_token *p_tkn, t_token *curr_token, t_token *nxt_tkn);
int			redirect_rules(t_token *curr_token, t_token *next_tkn, t_info *shl);

// interpreter
int			eval_tokens(t_info *shell_data);
int			init_pipe(t_command *expr);
void		copy_pipes_fds(int *dest, int *src);
char		*args_eval(char *arg, char **env);
int			alloc_cmd_fields(t_info *shell_data, int *field_count);
int			destroy_resources(t_info *shell_data);

// Error handling
int			print_err_msg(void);
void		print_err_str(char *err_msg);
void		print_syntax_err(char *op);
void		print_quote_err(int quote);
int			set_error(char *error_message, int error_code, char **args);

// Signals handling
void		handle_signals(void);
void		handle_child_signals(void);

// pipes
void		pipes_setup(t_command *expr);
void		pipes_close(t_command *expr);
void		pipes_builtin_setup(t_command *expr, int *std_backup);
void		pipes_builtin_close(t_command *expr, int *std_backup);

// redirection
int			file_open_read(char *filename, int *redirect);
int			file_open_write(char *filename, int *redirect, int mode);
char		*here_doc(char *delimiter);
int			init_here_doc(t_token *curr_token, t_token *next_tkn, t_info *s_dt);
void		redirect_setup(t_command *expr);
void		redirect_close(t_command *expr);
void		redirection_builtin_setup(t_command *expr, int *std_backup);
void		redirection_builtin_close(t_command *expr, int *std_backup);

// Quotes
char		*quote_resolver(char *str);
int			update_quote(char *str, int i, int *quote_flag);

// Expand variables
int			valid_variable(char *c);
char		*expand_str(char *input, char **env);
char		*str_nodes_join(t_list *str_nodes);
char		*expand_exit_variable(int *variable_size);

// Utils
int			is_builtin(char *cmd_path);
char		**build_env(char **env);
char		*gen_name(int id);
void		lst_pid_add_back(t_pid_l **pid_lst, int pid);

// cleaner
void		free2d(void **matrix2d);
void		free_token(void *tk);
void		reset_state(t_info *shell_data);
void		destroy_info(t_info *shell_data);
void		destroy_shell(t_info *shell_data);
#endif
