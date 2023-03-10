/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 14:16:14 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/22 16:26:04 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* 
 *	description: prints the argument on the STDOUT	
 *  usage: echo [-n] string1 string2 .... stringN
 *  options: -n [removes the trailing new line]
 *  return: 0 on success, 1 on failure [wrong args]
 */

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

int	is_flag(char **args, int *i)
{
	int		flag;
	int		j;
	int		k;

	flag = 0;
	j = 1;
	while (args[j])
	{
		k = 0;
		if (args[j][k] == '-')
			k++;
		while (args[j][k])
		{
			if (args[j][k] != 'n')
			{
				return (flag);
			}
			k++;
		}
		j++;
		*i += 1;
		flag = 1;
	}
	return (flag);
}

int	ft_echo(char **args)
{
	int	is_flag_valid;
	int	i;

	i = 0;
	while (args[i])
		i++;
	i = 1;
	is_flag_valid = 0;
	if (args[1] && !ft_strncmp(args[1], "-n", 2))
		is_flag_valid = is_flag(args, &i);
	if (is_flag_valid)
		print_args(args, i);
	else
	{
		print_args(args, 1);
		printf("\n");
	}
	g_exit_code = 0;
	return (g_exit_code);
}
