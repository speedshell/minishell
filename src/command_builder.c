/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_builder.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/14 17:10:25 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/14 17:53:38 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <fcntl.h>

int		redirect_open(t_command *expr, int *redirect, int *i);
void	add_next_arg(t_info *shell_data, int *i, int *j);
int		resolve_redirection(t_command *expr, char **cmd, int *i);
void	init_vars(char ***cmd, t_command **expr, t_info *shell_data);

char	**command_builder(t_info *shell_data)
{
	char		**cmd;
	t_command	*expr;
	int			field_count;
	int			i;
	int			j;

	if (alloc_cmd_fields(shell_data, &field_count) == 0)
		return (NULL);
	init_vars(&cmd, &expr, shell_data);
	i = 0;
	j = 0;
	while ((i < field_count && cmd[j] == NULL) \
		&& expr->tokens[i]->type != PIPE)
	{
		if (expr->tokens[i]->type == REDIRECT)
		{
			if (resolve_redirection(expr, cmd, &i) == -1)
				return (NULL);
		}
		else
			add_next_arg(shell_data, &i, &j);
	}
	cmd[0] = command_find_path(cmd[0], shell_data->env);
	cmd[j] = NULL;
	return (cmd);
}

int	resolve_redirection(t_command *expr, char **cmd, int *i)
{
	if (redirect_open(expr, expr->redirect, i) == -1)
	{
		free2d((void **) cmd);
		return (-1);
	}
	return (0);
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
	*i = *i + 2;
	return (op_code);
}

void	add_next_arg(t_info *shell_data, int *i, int *j)
{
	t_command	*expr;
	char		**cmd;

	expr = shell_data->expr;
	cmd = shell_data->cmd;
	cmd[*j] = args_eval(expr->tokens[*i]->value, shell_data->env);
	shell_data->expr->tokens[*i]->value = NULL;
	*i = *i + 1;
	*j = *j + 1;
}

void	init_vars(char ***cmd, t_command **expr, t_info *shell_data)
{
	*cmd = shell_data->cmd;
	*expr = shell_data->expr;
}
