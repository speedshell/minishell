/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kludge.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpinna-l <mpinna-l@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 09:28:32 by mpinna-l          #+#    #+#             */
/*   Updated: 2023/01/07 09:28:40 by mpinna-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_kludge(t_kludge *kludge, char *input);
void	expand_variable(t_kludge *kludge, char **env, char *input, int i);
void	copy_variable(t_kludge *kludge);

void	init_kludge(t_kludge *kludge, char *input)
{
	kludge->flag = 0;
	kludge->variable = NULL;
	kludge->j = 0;
	kludge->str = ft_calloc(ft_strlen(input) + 1, 1);
}

void	expand_variable(t_kludge *kludge, char **env, char *input, int i)
{
	kludge->temp = ft_substr(input, 1, i - 1);
	kludge->variable = env_search(kludge->temp, env, &(kludge->j));
	kludge->temp = kludge->str;
	kludge->str = ft_strjoin(kludge->str, kludge->variable);
	free(kludge->temp);
	free(kludge->variable);
	kludge->flag = 1;
}

void	copy_variable(t_kludge *kludge)
{
	kludge->temp = ft_calloc(ft_strlen(kludge->str) + 2, sizeof(char));
	ft_strlcpy(kludge->temp, kludge->str, ft_strlen(kludge->str) + 2);
	free(kludge->str);
	kludge->str = kludge->temp;
	kludge->flag = 0;
}
