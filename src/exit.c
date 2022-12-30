/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 17:47:37 by lfarias-          #+#    #+#             */
/*   Updated: 2022/12/30 10:45:37 by mpinna-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	convert_to_range(char *arg);

/*
*	description: will exit the shell and return a value in the 256 range
*	args: a single number
*	return: on exit returns zero or args % 256 if given 
*			on error it doesn't exit and returns 1
*/

int	ft_exit(char **args)
{
	int	i;
	int	exit_code;

	i = 1;
	while (args[i] != NULL)
		i++;
	printf("exit\n");
	if (i > 2)
	{
		// TO-DO: it should actually print to STDERR
		printf("exit: too many arguments\n");
		return (1);
	}
	exit_code = convert_to_range(args[1]);
	exit (exit_code);
	return (0);
}

/*
*	description: aux function, takes a valid argument and converts it
*				to a range
*/
int	convert_to_range(char *arg)
{
	long	number;

	if (arg == NULL)
		return (0);
	number = ft_atoi(arg);
	number = number % 256;
	return (number);
}