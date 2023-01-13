/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 22:43:16 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/13 13:53:40 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <fcntl.h>
#include <stdlib.h>

char	**command_builder(t_command *expr, char **env, int *redirect);
int		redirect_open(t_command *expr, int *redirect, int *i);
int		wait_children(void);
int		destroy_resources(t_command *expr, char **cmd);

int	eval_tokens(t_list **tokens, t_env *env_clone)
{
	t_command	*expr;
	char		**cmd;
	int			prev_pipe[2];
	t_list		*token_list;

	if (check_syntax(*tokens) != 1)
		return (-1);
	token_list = init_vars(&expr, &cmd, prev_pipe, tokens);
	while (token_list != NULL)
	{
		expr = parse_expression(&token_list);
		cmd = command_builder(expr, env_clone->env, expr->redirect);
		if (cmd == NULL)
			break ;
		copy_pipes_fds(expr->in_pipe, prev_pipe);
		if (init_pipe(expr) == -1)
		{
			destroy_resources(expr, cmd);
			return (-1);
		}
		command_executor(cmd, expr, env_clone);
		copy_pipes_fds(prev_pipe, expr->out_pipe);
		destroy_resources(expr, cmd);
	}
	return (wait_children());
}

int	wait_children(void)
{
	int	w_status;

	w_status = 0;
	while (42)
	{
		w_status = wait(&w_status);
		if (w_status <= 0)
			break ;
	}
	return (0);
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
	if (alloc_fields(expr, &field_count, &cmd) == 0)
		return (NULL);
	while (i < field_count && expr->tokens[i]->type != PIPE)
	{
		if (expr->tokens[i]->type == REDIRECT \
			&& redirect_open(expr, redirect, &i) == -1)
			return (NULL);
		else
		{
			cmd[j++] = args_eval(expr->tokens[i]->value, env);
			expr->tokens[i++]->value = NULL;
		}
	}
	cmd[0] = command_find_path(cmd[0], env);
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
	*i = *i + 2;
	return (op_code);
}

int	destroy_resources(t_command *expr, char **cmd)
{
	free(expr->tokens);
	free(expr);
	free2d((void **) cmd);
	return (0);
}
