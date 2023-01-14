/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpinna-l <mpinna-l@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 08:46:06 by mpinna-l          #+#    #+#             */
/*   Updated: 2023/01/14 15:58:43 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <unistd.h>

int		g_exit_code;

void	handle_eof(t_info *shell_data)
{
	if (!shell_data->read_line_buffer)
	{
		destroy_shell(shell_data);
		printf("exit\n");
		exit(0);
	}
	else
		reset_state(shell_data);
}

void	init_shell(t_info *shell_data, char **env)
{
	shell_data->read_line_buffer = NULL;
	shell_data->token_list = NULL;
	shell_data->cmd = NULL;
	shell_data->expr = NULL;
	shell_data->env = build_env(env);
	handle_signals();
}

int	main(int argc, char **argv, char **env)
{
	t_info		shell_data;

	g_exit_code = 0;
	(void)argv;
	if (argc != 1)
		return (set_error("Too many args. Usage: ./minishell\n", 1, NULL));
	init_shell(&shell_data, env);
	while (42)
	{
		shell_data.read_line_buffer = readline("Minishell> ");
		if (shell_data.read_line_buffer && *shell_data.read_line_buffer)
		{
			add_history(shell_data.read_line_buffer);
			shell_data.token_list = make_tokens(shell_data.read_line_buffer);
			g_exit_code = eval_tokens(&shell_data);
		}
		handle_eof(&shell_data);
		if (access(".here_doc", F_OK) == 0)
			unlink(".here_doc");
	}
	free2d((void **) shell_data.env);
	rl_clear_history();
	return (0);
}
