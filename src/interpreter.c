/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 22:43:16 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/17 20:02:04 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <fcntl.h>
#include <stdlib.h>

char	**command_builder(t_info *shell_data);
int		wait_children(int last_error);
int		get_next_command(t_list **token_list, t_info *shell_data, int *p_pipe);

int	eval_tokens(t_info *shell_data)
{
	int			prev_pipe[2];
	t_list		*token_list;
	int			last_error;	

	last_error = 0;
	if (check_syntax(shell_data->token_list, shell_data) != 1)
	{
		if (g_exit_code == 130)
			g_exit_code = 1;
		g_exit_code = 2;
		return (g_exit_code);
	}
	token_list = shell_data->token_list;
	prev_pipe[0] = -1;
	prev_pipe[1] = -1;
	while (token_list != NULL)
	{
		last_error = get_next_command(&token_list, shell_data, prev_pipe);
		command_executor(shell_data);
		copy_pipes_fds(prev_pipe, shell_data->expr->out_pipe);
		destroy_resources(shell_data);
	}
	return (wait_children(last_error));
}	

int	get_next_command(t_list **token_list, t_info *shell_data, int *prev_pipe)
{
	shell_data->expr = parse_expression(token_list);
	copy_pipes_fds(shell_data->expr->in_pipe, prev_pipe);
	if (init_pipe(shell_data->expr) == -1)
	{
		destroy_resources(shell_data);
		return (g_exit_code);
	}
	shell_data->cmd = command_builder(shell_data);
	if (shell_data->cmd == NULL)
	{
		return (g_exit_code);
	}
	return (0);
}

int	wait_children(int last_error)
{
	int	w_status;
	int	child_pid;

	w_status = 0;
	child_pid = 0;
	while (42)
	{
		child_pid = wait(&w_status);
		if (child_pid <= 0)
			break ;
		if (WIFEXITED(w_status) && last_error == 0)
		{
			g_exit_code = (int) WEXITSTATUS(w_status);
		}
	}
	if (last_error != 0)
		g_exit_code = last_error;
	return (g_exit_code);
}
