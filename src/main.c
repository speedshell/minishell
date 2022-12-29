/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpinna-l <mpinna-l@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 08:46:06 by mpinna-l          #+#    #+#             */
/*   Updated: 2022/12/29 14:26:46 by mpinna-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int argc, char **argv, char **env)
{
	char	*read_line_buffer;
	char	**cmd;

	(void)argv;
	if (argc != 1)
		return (1);
	handle_signals();
	while (42)
	{
		read_line_buffer = readline("Minishell> ");
		if (read_line_buffer && *read_line_buffer)
		{
			add_history(read_line_buffer);
			cmd = parse_command(read_line_buffer, env);
			command_executor(cmd[0], cmd, env);
			free2d((void **) cmd);
		}
		free(read_line_buffer);
	}
	rl_clear_history();
	return (0);
}
