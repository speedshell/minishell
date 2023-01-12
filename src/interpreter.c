/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 22:43:16 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/12 20:22:47 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <fcntl.h>
#include <stdlib.h>

char	**command_builder(t_command *expr, char **env, int *redirect);
char	*args_eval(char *arg, char **env);
int		alloc_fields(t_command *expr, int *field_count, char ***cmd);
int		redirect_open(t_command *expr, int *redirect, int *i);
int		wait_pipes(void);
void	copy_pipes_fds(int *dest, int *src);
int		init_pipe(t_command *expr);
int		destroy_resources(t_command *expr, char **cmd);

//printf("token value: %s\n", ((t_token *) (*tokens)->content)->value);
int	eval_tokens(t_list **tokens, t_env *env_clone)
{
	t_command	*expr;
	char		**cmd;
	int			prev_pipe[2];

	prev_pipe[0] = -1;
	prev_pipe[1] = -1;
	if (check_syntax(*tokens) != 0)
		return (-1);
	while (*tokens)
	{
		expr = parse_expression(tokens);
		if (expr == NULL)
			break ;
		cmd = command_builder(expr, env_clone->env, expr->redirect);
		if (cmd == NULL)
			break ;
		copy_pipes_fds(expr->in_pipe, prev_pipe);
		if (init_pipe(expr) == -1)
		{
			destroy_resources(expr, cmd);
			return (-1);	
		}
		cmd[0] = parse_command(cmd[0], env_clone->env);
		command_executor(cmd, expr, env_clone);
		copy_pipes_fds(prev_pipe, expr->out_pipe);
		destroy_resources(expr, cmd);
	}
	return (wait_pipes());
}

int	wait_pipes(void)
{
	int	w_status;

	w_status = 0;
	while (42)
	{
		w_status = wait(&w_status);
		if (w_status <= 0)
			break ;
	}
	return (0); // will return the value of the global return code;
}

int	init_pipe(t_command *expr)
{
	int op_code;

	op_code = 0;
	if (!expr->has_pipe)
		return (op_code);
	op_code = pipe(expr->out_pipe);
	if (op_code == -1)
	{
		print_err_msg();
		return (-1);
	}
	return (0);
}

int	destroy_resources(t_command *expr, char **cmd)
{
return (0);	
}

void	copy_pipes_fds(int *dest, int *src)
{
	dest[0] = src[0];
	dest[1] = src[1];
}

int	alloc_fields(t_command *expr, int *field_count, char ***cmd)
{
	int	i;

	if (!expr || expr->tokens == NULL)
		return (0);
	i = 0;
	while (expr->tokens[i] != NULL)
	{
		*field_count = ++i;
	}
	if (field_count == 0)
		return (0);
	*cmd = malloc(sizeof(char *) * (*field_count + 1));
	if (!cmd)
		return (0);
	else
		return (1);
}

char	**command_builder(t_command *expr, char **env, int *redirect)
{
	char	**cmd;
	int		field_count;
	int		i;
	int		j;

	i = 0;
	j = 0;
	field_count = 0;
	cmd = NULL;
	if (alloc_fields(expr, &field_count, &cmd) == 0)
		return (NULL);
	while (i < field_count && expr->tokens[i]->type != PIPE)
	{
		if (expr->tokens[i]->type == REDIRECT)
		{
			if (redirect_open(expr, redirect, &i) == -1)
				return (NULL);
		}
		else
		{
			cmd[j++] = args_eval(expr->tokens[i]->value, env);
			free(expr->tokens[i++]);
		}
	}
	free_token(expr->tokens[i]);
	free(expr->tokens);
	cmd[j] = NULL;
	return (cmd);
}

int	redirect_open(t_command *expr, int *redirect, int *i)
{
	char	*token_value;
	char	*filename;
	int		token_size;
	int		op_code;

	op_code = 0;
	token_value = expr->tokens[*i]->value;
	token_size = ft_strlen(token_value);
	filename = expr->tokens[*i + 1]->value;
	if (token_size == 1 && *token_value == '<')
		op_code = file_open_read(filename, redirect);
	if (token_size == 1 && *token_value == '>')
		op_code = file_open_write(filename, redirect, O_TRUNC);
	if (token_size == 2 && ft_strncmp(token_value, ">>", 2) == 0)
		op_code = file_open_write(filename, redirect, O_APPEND);
	if (token_size == 2 && ft_strncmp(token_value, "<<", 2) == 0)
		op_code = here_doc(filename, redirect);
	if (op_code == -1)
		return (op_code);
	free_token(expr->tokens[*i]);
	free_token(expr->tokens[*i + 1]);
	*i = *i + 2;
	return (op_code);
}

char	*args_eval(char *arg, char **env)
{
	char	*temp;

	temp = expand_str(arg, env);
	if (temp != NULL)
	{
		free(arg);
		arg = temp;
	}
	temp = quote_resolver(arg);
	if (temp != NULL)
	{
		free(arg);
		arg = temp;
	}
	return (arg);
}
