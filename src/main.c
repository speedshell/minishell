/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpinna-l <mpinna-l@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 08:46:06 by mpinna-l          #+#    #+#             */
/*   Updated: 2023/01/14 12:28:12 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <unistd.h>

int		g_exit_code;

void	destroy_info(t_info *shell_data)
{
	ft_lstclear(&shell_data->token_list, free_token);
	free(shell_data->expr);
	free2d((void **) shell_data->cmd);
}

void	handle_eof(char *input, t_info *shell_data)
{
	destroy_info(shell_data);
	if (!input)
	{
		printf("exit\n");
		exit(0);
	}
	else
		free(input);
}


int	main(int argc, char **argv, char **env)
{
	char		*read_line_buffer;
	t_info		shell_data;

	g_exit_code = 0;
	(void)argv;
	if (argc != 1)
		return (set_error("Too many args. Usage: ./minishell\n", 1, NULL));
	read_line_buffer = NULL;
	shell_data.token_list = NULL;
	shell_data.cmd = NULL;
	shell_data.expr = NULL;
	shell_data.env = build_env(env);
	handle_signals();
	while (42)
	{
		read_line_buffer = readline("Minishell> ");
		if (read_line_buffer && *read_line_buffer)
		{
			add_history(read_line_buffer);
			shell_data.token_list = make_tokens(read_line_buffer);
			g_exit_code = eval_tokens(&shell_data);
		}
		handle_eof(read_line_buffer, &shell_data);
		if (access(".here_doc", F_OK) == 0)
			unlink(".here_doc");
	}
	free2d((void **) shell_data.env);
	rl_clear_history();
	return (0);
}

