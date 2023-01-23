/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_resolver.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 18:25:25 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/23 16:33:05 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	search_next_quote(char *str, char c, int quote_flag);

int	update_quote(char *str, int i, int *quote_flag)
{
	if (str[i] == '\0')
		return (0);
	if ((*quote_flag == 0 && str[i] == '\'') && \
		search_next_quote(&str[i + 1], str[i], *quote_flag) == 1)
	{
		*quote_flag = 1;
		return (1);
	}
	if (*quote_flag == 1 && str[i] == '\'')
	{
		*quote_flag = 0;
		return (1);
	}
	if ((*quote_flag == 0 && str[i] == '"') && \
		search_next_quote(&str[i + 1], str[i], *quote_flag) == 1)
	{
		*quote_flag = 2;
		return (1);
	}
	if (*quote_flag == 2 && str[i] == '"')
	{
		*quote_flag = 0;
		return (1);
	}
	return (0);
}

int	check_for_quotes(char *input)
{
	int	i;
	int	quote;
	int	valid_quote;

	i = -1;
	quote = 0;
	valid_quote = 0;
	while (input[++i])
	{
		if (input[i] == '\'' && quote == 0)
			quote = 1;
		if (input[i] == '\'' && quote == 1)
		{
			quote = 0;
			valid_quote += 1;
		}
		if (input[i] == '\"' && quote == 0)
			quote = 1;
		if (input[i] == '\"' && quote == 1)
		{
			quote = 0;
			valid_quote += 1;
		}
	}
	return (valid_quote);
}

char	*quote_resolver(char *str)
{
	char	*parsed_str;
	int		quote_flag;
	int		i;
	int		j;

	if (!str || check_for_quotes(str) == 0)
		return (NULL);
	parsed_str = ft_calloc(sizeof(char), ft_strlen(str) + 1);
	if (!parsed_str)
		return (NULL);
	quote_flag = 0;
	i = 0;
	j = 0;
	while (str[i])
	{
		if (update_quote(str, i, &quote_flag) == 1)
		{
			i++;
			continue ;
		}
		parsed_str[j++] = str[i++];
	}
	parsed_str[j] = '\0';
	return (parsed_str);
}

int	search_next_quote(char *str, char c, int quote_flag)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c && quote_flag == 0)
			return (1);
		i++;
	}
	return (0);
}
