/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_rules.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 15:02:13 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/16 00:38:40 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token		*get_next_token(t_list *token_list);
void		check_quote(char *str, int *quote);

int	check_syntax(t_list *token_list)
{
	t_token	*tkn;
	t_token	*prev_tkn;
	t_token	*next_tkn;
	int		quote;

	quote = 0;
	prev_tkn = NULL;
	next_tkn = NULL;
	while (token_list)
	{
		tkn = (t_token *) token_list->content;
		check_quote(tkn->value, &quote);
		next_tkn = get_next_token(token_list);
		if (pipe_rules(prev_tkn, tkn, next_tkn) == -1 || \
				redirect_rules(tkn, next_tkn) == -1)
			return (0);
		prev_tkn = tkn;
		token_list = token_list->next;
	}
	if (quote != 0)
	{
		print_quote_err(quote);
		return (0);
	}
	return (1);
}

void	check_quote(char *str, int *quote)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (*quote == 0 && str[i] == '\'')
			*quote = 1;
		else if (*quote == 1 && str[i] == '\'')
			*quote = 0;
		else if (*quote == 0 && str[i] == '"')
			*quote = 2;
		else if (*quote == 2 && str[i] == '"')
			*quote = 0;
		i++;
	}
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
