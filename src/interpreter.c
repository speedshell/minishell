/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 22:43:16 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/14 16:31:32 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <fcntl.h>
#include <stdlib.h>

char	**command_builder(t_info *shell_data);
int		redirect_open(t_command *expr, int *redirect, int *i);
int		wait_children(void);
int		destroy_resources(t_info *shell_data);

int	eval_tokens(t_info *shell_data)
{
	int			prev_pipe[2];
	t_list		*token_list;

	if (check_syntax(shell_data->token_list) != 1)
		return (2);
	token_list = shell_data->token_list;
	prev_pipe[0] = -1;
	prev_pipe[1] = -1;
	while (token_list != NULL)
	{
		shell_data->expr = parse_expression(&token_list);
		shell_data->cmd = command_builder(shell_data);
		if (shell_data->cmd == NULL)
		{
			destroy_resources(shell_data);
			break ;
		}
		copy_pipes_fds(shell_data->expr->in_pipe, prev_pipe);
		if (init_pipe(shell_data->expr) == -1)
		{
			destroy_resources(shell_data);
			return (-1);
		}
		command_executor(shell_data);
		copy_pipes_fds(prev_pipe, shell_data->expr->out_pipe);
		destroy_resources(shell_data);
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

char	**command_builder(t_info *shell_data)
{
	char		**cmd;
	t_command	*expr;
	int			field_count;
	int			i;
	int			j;

	field_count = 0;
	if (alloc_cmd_fields(shell_data, &field_count) == 0)
		return (NULL);
	cmd = shell_data->cmd;
	expr = shell_data->expr;
	i = 0;
	j = 0;
	while ((i < field_count && cmd[j] == NULL) && expr->tokens[i]->type != PIPE)
	{
		if (expr->tokens[i]->type == REDIRECT) 
		{
			if (redirect_open(expr, expr->redirect, &i) == -1)
			{
				free2d((void **) cmd);
				return (NULL);
			}
		}
		else
		{
			cmd[j++] = args_eval(shell_data->expr->tokens[i]->value, shell_data->env);
			shell_data->expr->tokens[i++]->value = NULL;
		}
	}
	cmd[0] = command_find_path(cmd[0], shell_data->env);
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

int	destroy_resources(t_info *shell_data)
{
	if (shell_data->expr->tokens != NULL)
	{
		free(shell_data->expr->tokens);
		free(shell_data->expr);
		shell_data->expr = NULL;
	}
	if (shell_data->cmd != NULL)
	{
		free2d((void **) shell_data->cmd);
		shell_data->cmd = NULL;
	}
	return (0);
}
