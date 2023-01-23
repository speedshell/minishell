/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 13:34:26 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/23 16:32:47 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_list		*make_tokens(char *user_input);
t_command	*create_expression(void);
int			count_tokens(t_list *token_list);
void		set_expr_attr(t_token *tkn, t_command *cmd, int *is_pipe_chain);

t_command	*parse_expression(t_list **token_list)
{
	t_token		*tkn;
	t_command	*cmd;
	int			i;
	int			token_qty;
	static int	is_pipe_chain;

	token_qty = count_tokens(*token_list);
	if (token_qty == 0)
		return (NULL);
	cmd = create_expression();
	cmd->tokens = malloc(sizeof(t_token *) * (token_qty + 1));
	i = -1;
	while (++i < token_qty && *token_list)
	{
		tkn = (t_token *)(*token_list)->content;
		cmd->tokens[i] = tkn;
		set_expr_attr(tkn, cmd, &is_pipe_chain);
		*token_list = (*token_list)->next;
	}
	cmd->tokens[i] = NULL;
	if (*token_list == NULL || i > token_qty)
		is_pipe_chain = 0;
	return (cmd);
}

void	set_expr_attr(t_token *tkn, t_command *cmd, int *is_pipe_chain)
{
	if (tkn->type == PIPE)
	{
		cmd->has_pipe = 1;
		*is_pipe_chain = 1;
	}
	if (tkn->type == REDIRECT)
		cmd->has_redirect = 1;
	if (*is_pipe_chain)
		cmd->pipe_chain = 1;
}

t_command	*create_expression(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
		return (NULL);
	cmd->has_pipe = 0;
	cmd->pipe_chain = 0;
	cmd->has_redirect = 0;
	cmd->builtin = 0;
	cmd->redirect[0] = -1;
	cmd->redirect[1] = -1;
	cmd->in_pipe[0] = -1;
	cmd->in_pipe[1] = -1;
	cmd->out_pipe[0] = -1;
	cmd->out_pipe[1] = -1;
	return (cmd);
}

int	count_tokens(t_list *token_list)
{
	t_list	*node;
	t_token	*tkn;
	int		expression_size;

	node = token_list;
	expression_size = 0;
	while (node)
	{
		tkn = (t_token *) node->content;
		if (tkn->type == PIPE)
			return (++expression_size);
		expression_size++;
		node = node->next;
	}
	return (expression_size);
}
