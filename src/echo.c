/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 14:16:14 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/06 11:05:59 by mpinna-l         ###   ########.fr       */
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
		if (!ft_isalpha(str[j]) && j > 0)
			return (0);
		if (str[j] == hidden[i])
			i++;
		j++;
	}
	while (str[j])
	{
		if (!ft_isalpha(str[j++]))
			return (0);
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

int	is_flag(char *str)
{
	if (*str == '-')
		str++;
	while (*str)
	{
		if (*str != 'n')
			return (0);
		str++;
	}
	return (1);
}

int	ft_echo(char **args)
{
	int	is_flag_valid;

	is_flag_valid = 0;
	if (args[1] && !ft_strncmp(args[1], "-n", 2))
		is_flag_valid = is_flag(args[1]);
	if (is_flag_valid)
		print_args(args, 2);
	else
	{
		print_args(args, 1);
		printf("\n");
	}
	return (0);
}
