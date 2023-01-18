/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pid_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 16:55:50 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/18 18:09:18 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	lst_pid_add_back(t_pid_l **pid_lst, int pid)
{
	t_pid_l	*node;
	t_pid_l	*new_node;

	new_node = malloc(sizeof(*new_node));
	if (!new_node)
	{
		print_err_msg();
		return ;
	}
	new_node->pid = pid;
	new_node->next = NULL;
	if (*pid_lst == NULL)
	{
		*pid_lst = new_node;
		return ;
	}
	node = *(pid_lst);
	while (node->next != NULL)
		node = node->next;
	node->next = new_node;
}
