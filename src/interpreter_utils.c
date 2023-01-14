/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 21:49:14 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/14 11:29:40 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_list	*init_vars(t_command **expr, char ***cmd, int *p_pipe, t_list **tks)
{
	(void)expr;
	(void)cmd;
	expr = NULL;
	cmd = NULL;
	p_pipe[0] = -1;
	p_pipe[1] = -1;
	return (*tks);
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
