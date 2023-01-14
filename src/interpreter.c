/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 22:43:16 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/14 17:52:54 by lfarias-         ###   ########.fr       */
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
		return (2);
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

	w_status = 0;
	while (42)
	{
		w_status = wait(&w_status);
		if (w_status <= 0)
			break ;
	}
	return (0);
}
