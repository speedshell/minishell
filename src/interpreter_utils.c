/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 21:49:14 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/18 15:22:59 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

int	init_pipe(t_command *expr)
{
	int	op_code;

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

void	copy_pipes_fds(int *dest, int *src)
{
	dest[0] = src[0];
	dest[1] = src[1];
}

char	*args_eval(char *arg, char **env)
{
	char	*temp;

	if (!arg)
		return (NULL);
	temp = quote_resolver(arg);
	if (temp != NULL)
	{
		free(arg);
		arg = temp;
	}
	temp = expand_str(arg, env);
	if (temp != NULL)
	{
		free(arg);
		arg = temp;
	}
	return (arg);
}

int	alloc_cmd_fields(t_info *shell_data, int *field_count)
{
	int	i;

	shell_data->cmd = NULL;
	*field_count = 0;
	if (!shell_data->expr || shell_data->expr->tokens == NULL)
		return (0);
	i = 0;
	while (shell_data->expr->tokens[i] != NULL)
	{
		*field_count = ++i;
	}
	if (field_count == 0)
		return (0);
	shell_data->cmd = ft_calloc(sizeof(char *), (*field_count + 1));
	if (!shell_data->cmd)
		return (0);
	else
		return (1);
}
