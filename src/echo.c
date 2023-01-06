/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 14:16:14 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/06 00:18:47 by mpinna-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* 
 *	description: prints the argument on the STDOUT	
 *  usage: echo [-n] string1 string2 .... stringN
 *  options: -n [removes the trailing new line]
 *  return: 0 on success, 1 on failure [wrong args]
 */

int	ft_hiddenp(char *str, char *hidden)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[j] && hidden[i])
	{
		if (str[j] == hidden[i])
			i++;
		j++;
	}
	if (hidden[i] == '\0')
		return (1);
	else
		return (0);
}

void	print_args(char **args, int i)
{
	while (args[i] != NULL)
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
}

int	ft_echo(char **args)
{
	int	is_flag_valid;

	if (args[1])
		is_flag_valid = ft_hiddenp(args[1], "-n");
	if (is_flag_valid)
		print_args(args, 2);
	else
	{
		print_args(args, 1);
		printf("\n");
	}
	return (0);
}
