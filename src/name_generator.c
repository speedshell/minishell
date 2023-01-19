/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   name_generator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 16:09:00 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/17 19:02:07 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*convert_id(int id);

char	*gen_name(int id)
{
	char	*prefix;
	char	*number_suffix;
	char	*name;

	prefix = ".here_doc_";
	number_suffix = convert_id(id);
	name = ft_strjoin(prefix, number_suffix);
	free(number_suffix);
	return (name);
}

char	*convert_id(int id)
{
	int		len;
	char	*number_suffix;
	char	*zero_prefix;
	char	*temp;
	char	*pattern;

	pattern = "0000";
	number_suffix = ft_itoa(id);
	len = ft_strlen(number_suffix);
	if (len >= 4)
		return (number_suffix);
	else
	{
		zero_prefix = ft_substr(pattern, 0, 4 - len);
		temp = number_suffix;
		number_suffix = ft_strjoin(zero_prefix, number_suffix);
		free(zero_prefix);
		free(temp);
		return (number_suffix);
	}
}

/*int main(void)
{
	printf("file name: %s\n", gen_name(20));
	printf("file name: %s\n", gen_name(0));
	printf("file name: %s\n", gen_name(300));
	printf("file name: %s\n", gen_name(2000));
	printf("file name: %s\n", gen_name(20001));
}*/
