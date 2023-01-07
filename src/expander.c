/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpinna-l <mpinna-l@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 09:29:03 by mpinna-l          #+#    #+#             */
/*   Updated: 2023/01/07 09:29:06 by mpinna-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

#define ERR_MALLOC 0

char	*ft_strchr_plus(const char *s, int c)
{
	c = c % 256;
	while (*s && c != *s)
		s++;
	if (*s == c)
		return ((char *)(s + 1));
	return (0);
}

char	*env_search(char *variable, char **env, int *j)
{
	int		i;
	char	*temp;
	int		len;

	i = -1;
	temp = variable;
	variable = ft_strjoin(variable, "=");
	free(temp);
	if (!variable)
		return (0);
	len = ft_strlen(variable);
	while (env[++i])
	{
		if (!ft_strncmp(variable, env[i], len))
		{
			free(variable);
			temp = ft_strdup(ft_strchr_plus(env[i], '='));
			if (!temp)
				return (0);
			*j += ft_strlen(temp);
			return (temp);
		}
	}
	free(variable);
	return (ft_strdup(""));
}

int	is_expandable(char c, int i)
{
	if ((ft_isalpha(c) || c == '_') && i == 1)
		return (1);
	else if ((ft_isalpha(c) || ft_isdigit(c) || c == '_'))
		return (1);
	return (0);
}

char	*expanded_str(char *input, char **env)
{
	int			i;
	t_kludge	kludge;

	init_kludge(&kludge, input);
	if (!kludge.str)
		return (ERR_MALLOC);
	while (*input)
	{
		if (*input == '$' && *(input + 1))
		{
			i = 1;
			while (is_expandable(input[i], i))
				i++;
			expand_variable(&kludge, env, input, i);
			input += i;
		}
		else
		{
			copy_variable(&kludge);
			kludge.str[kludge.j++] = *input++;
		}
	}
	if (kludge.flag == 0)
		kludge.str[kludge.j] = 0;
	return (kludge.str);
}
