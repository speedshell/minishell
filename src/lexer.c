/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 18:33:43 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/23 16:32:10 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_operator(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}

void	quote_interruptor(char c, int *is_on)
{
	if (c == '\'' && *is_on == 0)
		*is_on = 1;
	else if (c == '\'' && *is_on == 1)
		*is_on = 0;
	else if (c == '"' && *is_on == 0)
		*is_on = 2;
	else if (c == '"' && *is_on == 2)
		*is_on = 0;
}

int	get_token_size(char *str)
{
	int	i;
	int	quote_on;

	quote_on = 0;
	i = 0;
	if (str[i] && is_operator(str[i]))
	{	
		if ((str[i] == '<' && str[i + 1] == str[i]) \
			|| (str[i] == '>' && str[i + 1] == str[i]))
		{
			i++;
		}
		i++;
		return (i);
	}
	while (str[i] && \
		(quote_on || (!is_operator(str[i]) && !ft_isspace(str[i]))))
	{
		quote_interruptor(str[i], &quote_on);
		i++;
	}
	return (i);
}

t_token	*create_token(char *str, int i, int token_size)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (str[i] == '|' && token_size == 1)
		token->type = PIPE;
	else if (str[i] == '<' && token_size == 1)
		token->type = REDIRECT;
	else if (str[i] == '>' && token_size == 1)
		token->type = REDIRECT;
	else if (token_size == 2 && ft_strncmp(&str[i], "<<", token_size) == 0)
		token->type = REDIRECT;
	else if (token_size == 2 && ft_strncmp(&str[i], ">>", token_size) == 0)
		token->type = REDIRECT;
	else
		token->type = WORD;
	token->value = ft_substr(str, i, token_size);
	return (token);
}

t_list	*make_tokens(char *user_input)
{
	t_list	*tokens;
	int		i;
	int		token_length;
	t_token	*token;

	if (!user_input)
		return (NULL);
	i = 0;
	tokens = NULL;
	while (user_input[i])
	{
		token_length = get_token_size(&user_input[i]);
		if (token_length != 0)
		{
			token = create_token(user_input, i, token_length);
			ft_lstadd_back(&tokens, ft_lstnew(token));
			i += token_length;
		}
		else
			i++;
	}
	return (tokens);
}
