/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 11:08:21 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/14 16:26:13 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free2d(void **matrix2d)
{
	int	i;

	if (!matrix2d)
		return ;
	i = 0;
	while (matrix2d[i] != NULL)
	{
		free(matrix2d[i]);
		i++;
	}
	free(matrix2d);
}

void	free_token(void *tk)
{
	t_token		*token;

	token = (t_token *) tk;
	if (token)
	{
		if (token->value)
			free(token->value);
		free(token);
	}
}

void	reset_state(t_info *shell_data)
{
	ft_lstclear(&shell_data->token_list, free_token);
	shell_data->token_list = NULL;
	if (shell_data->expr)
		free(shell_data->expr->tokens);
	free(shell_data->expr);
	shell_data->expr = NULL;
	free2d((void **) shell_data->cmd);
	shell_data->cmd = NULL;
	if (shell_data->read_line_buffer)
		free(shell_data->read_line_buffer);
}

void	destroy_shell(t_info *shell_data)
{
	reset_state(shell_data);
	free2d((void **) shell_data->env);
	rl_clear_history();
}
