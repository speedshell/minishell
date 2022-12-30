/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpinna-l <mpinna-l@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 08:46:06 by mpinna-l          #+#    #+#             */
/*   Updated: 2022/12/30 11:57:33 by mpinna-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_eof(char *input)
{
	free(input);
	if (!input)
	{
		printf("exit\n");
		exit(0);
	}
}

int	main(int argc, char **argv, char **env)
{
	char	*read_line_buffer;
	char	**cmd;
	t_env	env_clone;

	(void)argv;
	if (argc != 1)
		return (1);
	handle_signals();
	env_clone.env = build_env(env);
	while (42)
	{
		read_line_buffer = readline("Minishell> ");
		if (read_line_buffer && *read_line_buffer)
		{
			add_history(read_line_buffer);
			cmd = parse_command(read_line_buffer, env_clone.env);
			command_executor(cmd[0], cmd, &env_clone);
			free2d((void **) cmd);
		}
		handle_eof(read_line_buffer);
	}
	rl_clear_history();
	return (0);
}
