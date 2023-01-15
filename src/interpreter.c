/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 22:43:16 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/14 22:51:23 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <fcntl.h>
#include <stdlib.h>

char	**command_builder(t_info *shell_data);
int		wait_children(void);
int		get_next_command(t_list **token_list, t_info *shell_data, int *p_pipe);

int	eval_tokens(t_info *shell_data)
{
	int			prev_pipe[2];
	t_list		*token_list;

	if (check_syntax(shell_data->token_list) != 1)
	{
		g_exit_code = 2;
		return (2);
	}
	token_list = shell_data->token_list;
	prev_pipe[0] = -1;
	prev_pipe[1] = -1;
	while (token_list != NULL)
	{
		if (get_next_command(&token_list, shell_data, prev_pipe) == -1)
			continue ;
		command_executor(shell_data);
		copy_pipes_fds(prev_pipe, shell_data->expr->out_pipe);
		destroy_resources(shell_data);
	}
	return (wait_children());
}	

int	get_next_command(t_list **token_list, t_info *shell_data, int *prev_pipe)
{
	shell_data->expr = parse_expression(token_list);
	shell_data->cmd = command_builder(shell_data);
	if (shell_data->cmd == NULL)
	{
		destroy_resources(shell_data);
		return (-1);
	}
	copy_pipes_fds(shell_data->expr->in_pipe, prev_pipe);
	if (init_pipe(shell_data->expr) == -1)
	{
		destroy_resources(shell_data);
		return (-1);
	}
	return (0);
}

int	wait_children(void)
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
		if (WIFSIGNALED(w_status))
		{
			g_exit_code = (int) WTERMSIG(w_status);
			printf("hey\n");	
		}
	}
	return (0);
}
