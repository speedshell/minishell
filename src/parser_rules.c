/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_rules.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 15:02:13 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/06 15:04:38 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
		printf("syntax error near '|'\n");
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
		printf("syntax error near '<' or '>' \n");
	return (syntax);
}
