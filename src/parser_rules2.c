/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_rules2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 19:13:20 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/17 19:54:33 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	init_here_doc(t_token *curr_token, t_token *next_tkn, t_info *shell_data)
{
	char	*temp;
	char	*here_doc_file;

	free(curr_token->value);
	curr_token->value = ft_strdup("<");
	temp = next_tkn->value;
	here_doc_file = here_doc(next_tkn->value);
	if (here_doc_file != NULL)
	{
		free(temp);
		next_tkn->value = here_doc_file;
		if (shell_data->tmp_files == NULL)
			shell_data->tmp_files = ft_lstnew(here_doc_file);
		else
			ft_lstadd_back(&shell_data->tmp_files, ft_lstnew(here_doc_file));
	}
	else
		return (-130);
	return (1);
}
