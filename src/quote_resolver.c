/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_resolver.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 18:25:25 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/03 19:22:11 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	update_quote(char *str, int i, int *quote_flag)
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
	parsed_str = malloc(sizeof(char) * ft_strlen(str) + 1);
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
		parsed_str[j++] = str[i++];
	}
	parsed_str[j] = '\0';
	return (parsed_str);
}

/*int	main(void)
{

	char *test1 = "\"'\"Hello\"'\"";
	char *test2 = "\'\"\"Hello\"\"\'";	
	char *test3 = "\'\"\'Hello\'\"\'";
	char *test4 = "\'\"\'\'Hello\'\'\"\'";
	char *test5 = "\'Hello\'";
	char *test6 = "\"\"Hello\"\"";
	test1 = quote_resolver(test1);
	test2 = quote_resolver(test2);
	test3 = quote_resolver(test3);
	test4 = quote_resolver(test4);
	test5 = quote_resolver(test5);
	test5 = quote_resolver(test6);
	printf("%s\n", test1);
	printf("%s\n", test2);
	printf("%s\n", test3);
	printf("%s\n", test4);
	printf("%s\n", test5);
	printf("%s\n", test6);
}*/