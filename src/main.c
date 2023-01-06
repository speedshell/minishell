/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpinna-l <mpinna-l@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 08:46:06 by mpinna-l          #+#    #+#             */
/*   Updated: 2023/01/06 14:47:47 by lfarias-         ###   ########.fr       */
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
			input = expanded_str(read_line_buffer, env_clone.env);
			tokens = make_tokens(input);	
			free(input);
			eval_tokens(&tokens, &env_clone);
		}
		handle_eof(read_line_buffer);
	}
	rl_clear_history();
	return (0);
}
