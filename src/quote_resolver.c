/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_resolver.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 18:25:25 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/02 20:16:57 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int		update_quote(char *str, int i, int *quote_flag)
{
	if (str[i] == '\0')
		return (0);
	if (*quote_flag == 0 && str[i] == '\'')
	{
		*quote_flag = 1;
		return (1);
	}
	if (*quote_flag == 1 && str[i] == '\'')
	{
		*quote_flag = 0;
		return (1);
	}
	if (*quote_flag == 0 && str[i] == '"')
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

char	*quote_resolver(char *str)
{
	char	*parsed_str;
	int		quote_flag;
	int		i;
	int		j;

	if (!str)
		return (NULL);
	parsed_str = malloc(sizeof(char) * strlen(str) + 1);
	if (!str)
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
		parsed_str[j] = str[i];
		i++;
		j++;
	}
	parsed_str[j] = '\0';
	return (parsed_str);
}

#include <stdio.h>

int main(void)
{

	char *test1 = "\"'\"Hello\"'\"";
	
	test1 = quote_resolver(test1);
	printf("%s\n", test1);
}

