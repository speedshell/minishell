/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpinna-l <mpinna-l@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 08:46:06 by mpinna-l          #+#    #+#             */
/*   Updated: 2022/12/29 20:06:50 by mpinna-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int argc, char **argv, char **env)
{
	char	*read_line_buffer;
	char	**cmd;
	char	**clone_env;

	(void)argv;
	if (argc != 1)
		return (1);
	handle_signals();
	clone_env = build_env(env);
	while (42)
	{
		read_line_buffer = readline("Minishell> ");
		if (read_line_buffer && *read_line_buffer)
		{
			add_history(read_line_buffer);
			cmd = parse_command(read_line_buffer, clone_env);
			command_executor(cmd[0], cmd, clone_env);
			free2d((void **) cmd);
		}
		free(read_line_buffer);
	}
	rl_clear_history();
	return (0);
}
