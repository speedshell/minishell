/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 22:43:16 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/06 02:40:08 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**command_builder(t_command *expr);
int		count_fields(t_command *expr);

void	eval_tokens(t_list **tokens, t_env *env_clone)
{
	t_command	*expr;
	int			syntax;
	char		**cmd;

	syntax = check_syntax(*tokens);
	(void)env_clone;
	if (syntax == 0)
		return ;
	while (tokens)
	{
		//printf("token value: %s\n", ((t_token *) (*tokens)->content)->value);
		expr = parse_expression(tokens);
		if (expr == NULL)
			break ;
		cmd = command_builder(expr);
		if (cmd == NULL)
			break ;
		cmd[0] = parse_command(cmd[0], env_clone->env);
		command_executor(cmd[0], cmd, env_clone);
	}
}

int		count_fields(t_command *expr)
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

char	**command_builder(t_command *expr)
{
	char	**cmd;
	int		field_count;
	int		i;

	field_count = count_fields(expr);
	if (field_count == 0)
		return (NULL);
	cmd = malloc(sizeof(char *) * (field_count + 1));
	if (!cmd)
		return (NULL);
	i = 0;	
	while (expr->tokens[i] != NULL)
	{
		cmd[i] = expr->tokens[i]->value;
		i++;
	}
	cmd[i] = NULL;
	return (cmd);
}
