/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_rules.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 15:02:13 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/15 18:54:47 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*get_next_token(t_list *token_list);

int	check_syntax(t_list *token_list)
{
	t_token	*tkn;
	t_token	*prev_tkn;
	t_token	*next_tkn;

	prev_tkn = NULL;
	next_tkn = NULL;
	while (token_list)
	{
		tkn = (t_token *) token_list->content;
		next_tkn = get_next_token(token_list);
		if (pipe_rules(prev_tkn, tkn, next_tkn) == -1)
			return (0);
		if (redirect_rules(tkn, next_tkn) == -1)
			return (0);
		prev_tkn = tkn;
		token_list = token_list->next;
	}
	return (1);
}

int	pipe_rules(t_token *prev_tkn, t_token *curr_token, t_token *next_tkn)
{
	int		syntax;

	syntax = 0;
	if (curr_token->type == PIPE)
	{
		if (!(prev_tkn != NULL && next_tkn != NULL))
		{
			syntax = -1;
		}
		else if ((prev_tkn->type == WORD && \
			(next_tkn->type == WORD || next_tkn->type == REDIRECT)))
		{
			syntax = 1;
		}
		else
		{
			syntax = -1;
		}
	}
	else
		return (syntax);
	if (syntax == -1)
		print_syntax_err(curr_token->value);
	return (syntax);
}

int	redirect_rules(t_token *curr_token, t_token *next_tkn)
{
	int	syntax;

	syntax = 0;
	if (curr_token->type == REDIRECT)
	{
		if (!next_tkn)
			syntax = -1;
		else if (next_tkn->type == WORD)
			syntax = 1;
		else
			syntax = -1;
	}	
	else
		return (syntax);
	if (syntax == -1)
		print_syntax_err(curr_token->value);
	return (syntax);
}

t_token	*get_next_token(t_list *token_list)
{
	t_token	*tkn;

	tkn = NULL;
	if (token_list->next != NULL)
	{
		tkn = (t_token *) token_list->next->content;
	}
	return (tkn);
}
