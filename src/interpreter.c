/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 22:43:16 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/07 22:44:49 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**command_builder(t_command *expr, char **env);
char	*args_eval(char *arg, char **env);
int		count_fields(t_command *expr);

//printf("token value: %s\n", ((t_token *) (*tokens)->content)->value);
void	eval_tokens(t_list **tokens, t_env *env_clone)
{
	t_command	*expr;
	int			syntax;
	char		**cmd;
	int			prev_pipe[2];

	prev_pipe[0] = -1;
	prev_pipe[1] = -1;
	syntax = check_syntax(*tokens);
	if (syntax == 0)
		return ;
	while (*tokens)
	{
		expr = parse_expression(tokens);
		if (expr == NULL)
			break ;
		cmd = command_builder(expr, env_clone->env);
		if (cmd == NULL)
			break ;
		expr->in_pipe[0] = prev_pipe[0];
		expr->in_pipe[1] = prev_pipe[1];
		if (expr->has_pipe)
		{
			pipe(expr->out_pipe);
		}
		cmd[0] = parse_command(cmd[0], env_clone->env);
		command_executor(cmd, expr, env_clone);
		prev_pipe[0] = expr->out_pipe[0];
		prev_pipe[1] = expr->out_pipe[1];
		free(expr);
		free2d((void **) cmd);
	}
}

int	count_fields(t_command *expr)
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

char	**command_builder(t_command *expr, char **env)
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
	while (expr->tokens[i] != NULL && expr->tokens[i]->type != PIPE)
	{
		cmd[i] = args_eval(expr->tokens[i]->value, env);
		free(expr->tokens[i]);
		i++;
	}
	if (expr->tokens[i])
	{
		free(expr->tokens[i]->value);
		free(expr->tokens[i]);
	}
	free(expr->tokens);
	cmd[i] = NULL;
	return (cmd);
}

char	*args_eval(char *arg, char **env)
{
	char	*temp;

	(void)env;
	temp = quote_resolver(arg);
	if (temp != NULL)
	{
		free(arg);
		arg = temp;
	}
	return (arg);
}
