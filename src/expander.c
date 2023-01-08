/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpinna-l <mpinna-l@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 09:29:03 by mpinna-l          #+#    #+#             */
/*   Updated: 2023/01/08 02:35:28 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

#define ERR_MALLOC 0

int	valid_variable(char *c)
{
	int	i;

	i = 0;
	while (c[i] && c[i] != '$')
	{
		if (c[i + 1] && c[i + 1] == ' ')
			return (i);
		if (c[i] == '=' && i == 0)
			return (0);
		else if (c[i] == '=' && i != 0)
			break ;
		else if ((!ft_isalpha(c[i]) && !(c[i] == '_')) && i == 0)
			return (0);
		else if ((!ft_isalpha(c[i]) && !ft_isdigit(c[i]) && !(c[i] == '_')))
			return (0);
		i++;
	}
	return (i);
}

char	*expand_variable(char *input, char **env, int *variable_size)
{
	int		i;
	char	*variable;

	i = 0;
	if (!input)
		return (NULL);
	if (*input == '$')
	{
		input++;
		*variable_size = valid_variable(input);
		if (*variable_size != 0)
		{
			while (env[++i])
			{
				if (!ft_strncmp(input, env[i], *variable_size))
				{
					variable = ft_strdup(ft_strchr(env[i], '=') + 1);
					return (variable);
				}
			}
		}	
	}
	return (NULL);
}

char	*str_nodes_join(t_list *str_nodes)
{
	int		str_size;
	t_list	*node;
	char	*expanded_str;

	node = str_nodes;
	while (node)
	{
		str_size += ft_strlen(((char *) node->content));
		node = node->next;
	}
	expanded_str = ft_calloc(sizeof(char), str_size);
	while (str_nodes)
	{
		ft_strlcat(expanded_str, (char *) str_nodes->content, str_size);
		node = str_nodes;
		str_nodes = str_nodes->next;
		free(node->content);
		free(node);
	}
	return (expanded_str);
}

char *expand_str(char *input, char **env)
{
	t_list	*str_nodes;
	int		variable_size;
	char	*variable;
	int		i;
	int		j;

	i = 0;
	j = 0;
	variable_size = 0;
	str_nodes = NULL;
	while (input[i])
	{
		variable = expand_variable(&input[i], env, &variable_size);
		if (variable != NULL)
		{
			if ((i - j) > 0)
				ft_lstadd_back(&str_nodes, ft_lstnew(ft_substr(input, j, (i - j))));
			ft_lstadd_back(&str_nodes, ft_lstnew(variable));
			i += (variable_size + 2);
			j = i;
		}
		else
		{
			ft_lstadd_back(&str_nodes, ft_lstnew(ft_strdup("")));
			i += variable_size;
		}
		if (input[i + 1] == '\0')
		{
			ft_lstadd_back(&str_nodes, ft_lstnew(ft_substr(input, j, i - j)));
			i++;
		}
	}
	if (!str_nodes)
		return (NULL);
	return (str_nodes_join(str_nodes));	
}

int main (int argc, char **argv, char **env)
{
	char *var = "$PWD";
	char *var2 = "$PDW";
	char *var3 = "$#sp";
	char *var4 = "$123";
	char *var5 = "$HOME";
	char *var6 = "$HOME$";
	char *var7 = "$HOME abc $ABC $abc";

	printf("%s expanded -> %s\n", var, expand_str(var, env));
	printf("%s expanded -> %s\n", var2, expand_str(var2, env));
	printf("%s expanded -> %s\n", var3, expand_str(var3, env));
	printf("%s expanded -> %s\n", var4, expand_str(var4, env));
	printf("%s expanded -> %s\n", var5, expand_str(var5, env));
	printf("%s expanded -> %s\n", var6, expand_str(var6, env));
	printf("%s expanded -> %s\n", var7, expand_str(var7, env));
}
