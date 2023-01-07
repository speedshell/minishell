/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpinna-l <mpinna-l@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 09:29:03 by mpinna-l          #+#    #+#             */
/*   Updated: 2023/01/07 19:29:07 by mpinna-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

#define ERR_MALLOC 0

int	valid_variable(char *c)
{
	int	i;

	i = -1;
	while (c[++i])
	{
		if (c[i] == '=' && i == 0)
			return (0);
		else if (c[i] == '=' && i != 0)
			break ;
		else if ((!ft_isalpha(c[i]) && !(c[i] == '_')) && i == 0)
			return (0);
		else if ((!ft_isalpha(c[i]) && !ft_isdigit(c[i]) && !(c[i] == '_')))
			return (0);
	}
	return (1);
}

char	*expand_variable(char *input, char **env)
{
	int		i;
	char	*variable;

	i = 0;
	if (!input)
		return (NULL);
	if (*input == '$')
	{
		input++;
		if (valid_variable(input))
		{
			while (env[++i])
			{
				if (!ft_strncmp(input, env[i], ft_strlen(input)))
				{
					variable = ft_strdup(ft_strchr(env[i], '=') + 1);
					return (variable);
				}
			}
		}	
	}
	return (ft_strdup(""));
}
/*
int main (int argc, char **argv, char **env)
{
	char *var = "$PWD";
	char *var2 = "$PDW";
	char *var3 = "$#sp";
	char *var4 = "$123";
	char *var5 = "$HOME";
	char *var6 = "$HOME$";

	printf("%s expanded -> %s\n", var, expand_variable(var, env));
	printf("%s expanded -> %s\n", var2, expand_variable(var2, env));
	printf("%s expanded -> %s\n", var3, expand_variable(var3, env));
	printf("%s expanded -> %s\n", var4, expand_variable(var4, env));
	printf("%s expanded -> %s\n", var5, expand_variable(var5, env));
	printf("%s expanded -> %s\n", var6, expand_variable(var6, env));
}*/
