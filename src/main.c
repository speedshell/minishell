/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpinna-l <mpinna-l@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 08:46:06 by mpinna-l          #+#    #+#             */
/*   Updated: 2023/01/03 19:38:20 by lfarias-         ###   ########.fr       */
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
	char	*unquoted_line;
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
			unquoted_line = quote_resolver(read_line_buffer);
			free(read_line_buffer);
			cmd = parse_command(unquoted_line, env_clone.env);
			command_executor(cmd[0], cmd, &env_clone);
			free2d((void **) cmd);
		}
		handle_eof(unquoted_line);
	}
	rl_clear_history();
	return (0);
}
