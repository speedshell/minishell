/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 22:43:16 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/10 14:47:57 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <fcntl.h>

char	**command_builder(t_command *expr, char **env, int *redirect);
char	*args_eval(char *arg, char **env);
int		count_fields(t_command *expr);
int		redirect_open(t_command *expr, int *redirect, int *i);

//printf("token value: %s\n", ((t_token *) (*tokens)->content)->value);
void	eval_tokens(t_list **tokens, t_env *env_clone)
{
	t_command	*expr;
	int			syntax;
	char		**cmd;
	int			prev_pipe[2];
	int			redirect[2];

	prev_pipe[0] = -1;
	prev_pipe[1] = -1;
	syntax = check_syntax(*tokens);
	if (syntax == 0)
		return ;
	while (*tokens)
	{
		redirect[0] = -1;
		redirect[1] = -1;
		expr = parse_expression(tokens);
		if (expr == NULL)
			break ;
		cmd = command_builder(expr, env_clone->env, redirect);
		if (cmd == NULL)
			break ;
		expr->in_pipe[0] = prev_pipe[0];
		expr->in_pipe[1] = prev_pipe[1];
		if (expr->has_pipe)
		{
			pipe(expr->out_pipe);
		}
		cmd[0] = parse_command(cmd[0], env_clone->env);
		command_executor(cmd, expr, env_clone, redirect);
		prev_pipe[0] = expr->out_pipe[0];
		prev_pipe[1] = expr->out_pipe[1];
		free(expr);
		free2d((void **) cmd);
	}
}

int	count_fields(t_command *expr)
{
	int		i;

	if (!expr || expr->tokens == NULL)
		return (0);
	i = 0;
	while (expr->tokens[i] != NULL)
	{
		i++;
	}
	return (i);
}

char	**command_builder(t_command *expr, char **env, int *redirect)
{
	char	**cmd;
	int		field_count;
	int		i;
	int		j;

	field_count = count_fields(expr);
	if (field_count == 0)
		return (NULL);
	cmd = malloc(sizeof(char *) * (field_count + 1));
	if (!cmd)
		return (NULL);
	i = 0;
	j = 0;
	while (i < field_count && expr->tokens[i]->type != PIPE)
	{
		if (expr->tokens[i]->type == REDIRECT)
			redirect_open(expr, redirect, &i);
		else
		{
			cmd[j++] = args_eval(expr->tokens[i]->value, env);
			free(expr->tokens[i++]);
		}
	}
	if (expr->tokens[i])
	{
		free(expr->tokens[i]->value);
		free(expr->tokens[i]);
	}
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
	{
		op_code = file_open_read(filename, redirect);
	}
	if (token_size == 1 && *token_value == '>')	
	{
		op_code = file_open_write(filename, redirect);
	}
	if (op_code == -1)
		return (op_code);
	*i = *i + token_size + 1;
	return (0);
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
