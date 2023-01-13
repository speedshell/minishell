/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 11:08:21 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/13 11:18:13 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free2d(void **matrix2d)
{
	int	i;

	if (!matrix2d)
		return ;
	i = 0;
	while (matrix2d[i] != NULL)
	{
		free(matrix2d[i]);
		i++;
	}
	free(matrix2d);
}

void	free_token(void *tk)
{
	t_token		*token;

	token = (t_token *) tk;
	if (token)
	{
		if (token->value)
			free(token->value);
		free(token);
	}
}
