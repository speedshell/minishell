/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpinna-l <mpinna-l@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 08:46:06 by mpinna-l          #+#    #+#             */
/*   Updated: 2023/01/11 16:26:49 by lfarias-         ###   ########.fr       */
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
	char		*read_line_buffer;
	t_list		*tokens;
	t_env		env_clone;
	char		*input;

	g_exit_code = 0;
	(void)argv;
	if (argc != 1)
		return (set_error("Too many args. Usage: ./minishell\n", 0));
	handle_signals();
	env_clone.env = build_env(env);
	while (42)
	{
		read_line_buffer = readline("Minishell> ");
		if (read_line_buffer && *read_line_buffer)
		{
			add_history(read_line_buffer);
			input = read_line_buffer;
			tokens = make_tokens(input);
			eval_tokens(&tokens, &env_clone);
		}
		handle_eof(read_line_buffer);
	}
	rl_clear_history();
	return (0);
}
