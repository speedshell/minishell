/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/08 20:55:20 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/11 16:36:00 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*str_nodes_join(t_list *str_nodes)
{
	int		str_size;
	t_list	*node;
	char	*expanded_str;

	str_size = 0;
	node = str_nodes;
	while (node)
	{
		str_size += ft_strlen(((char *) node->content));
		node = node->next;
	}
	expanded_str = ft_calloc(sizeof(char), (str_size + 1));
	while (str_nodes)
	{
		ft_strlcat(expanded_str, (char *) str_nodes->content, str_size + 1);
		node = str_nodes;
		str_nodes = str_nodes->next;
		free(node->content);
		free(node);
	}
	return (expanded_str);
}

char	*expand_exit_variable(int *variable_size)
{
	*variable_size = 2;
	return (ft_itoa(g_exit_code));
}
