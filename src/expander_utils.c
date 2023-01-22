/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/08 20:55:20 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/22 15:47:37 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*str_nodes_join(t_list *str_nodes)
{
	int		str_sz;
	t_list	*node;
	char	*expanded_str;

	str_sz = 0;
	node = str_nodes;
	while (node)
	{
		if (node->content != NULL)
			str_sz += ft_strlen(((char *) node->content));
		node = node->next;
	}
	expanded_str = ft_calloc(sizeof(char), (str_sz + 1));
	while (str_nodes)
	{
		if (str_nodes->content != NULL)
			ft_strlcat(expanded_str, (char *) str_nodes->content, str_sz + 1);
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
