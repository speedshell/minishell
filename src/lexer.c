/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 18:33:43 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/05 13:19:10 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_redirect(char *str)
{
	if (*str == '<' && *(str + 1) == '<')
		return (1);
	if (*str == '>' && *(str + 1) == '>')
		return (1);
	return (0);
}

int	is_operator(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}

int	get_token_size(char *str)
{
	int	i;

	i = 0;
	if (str[i] && is_operator(str[i]))
	{	
		if ((str[i] == '<' && str[i + 1] == str[i]) \
			|| (str[i] == '>' && str[i + 1] == str[i]))
		{
			i++;
		}
		i++;
		return (i);
	}
	while (str[i] && (!is_operator(str[i]) && !ft_isspace(str[i])))
		i++;
	return (i);
}

t_token	*create_token(char *str, int i, int token_size, int *is_cmd)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (str[i] == '|' && token_size == 1)
	{
		token->type = "PIPE";
		*is_cmd = 0;
	}
	else if (str[i] == '<' && token_size == 1)
		token->type = "INPUT_REDIRECT";
	else if (str[i] == '>' && token_size == 1)
		token->type = "OUTPUT_REDIRECT";
	else if (token_size == 2 && ft_strncmp(&str[i], "<<", token_size) == 0)
		token->type = "HERE_DOC";
	else if (token_size == 2 && ft_strncmp(&str[i], ">>", token_size) == 0)
			token->type = "OUTPUT_APPEND";
	else if (*is_cmd == 0)
	{
		token->type = "COMMAND";
		*is_cmd = 1;
	}
	else
		token->type = "WORD";
	token->value = ft_substr(str, i, token_size);
	return (token);
}

t_list	*make_tokens(char *user_input)
{
	t_list	*tokens;
	int		i;
	int		token_length;
	int		is_cmd;
	t_token	*token;

	if (!user_input)
		return (NULL);
	i = 0;
	tokens = NULL;
	is_cmd = 0;
	while (user_input[i])
	{
		token_length = get_token_size(&user_input[i]);
		if (token_length != 0)
		{
			token = create_token(user_input, i, token_length, &is_cmd);
			ft_lstadd_back(&tokens, ft_lstnew(token));
			i += token_length;
		}
		else
			i++;
	}
	return (tokens);
}

void print_tokens(t_list *token_list)
{
	int		token_size;
	t_token *tkn;
	t_list	*aux_node;

	printf("====================================\n");
	while (token_list)
	{
		tkn = (t_token *) token_list->content;
		printf("token type: %s\n", tkn->type);
		printf("token value: %s\n", tkn->value);
		free(tkn->value);
		free(tkn);
		aux_node = token_list;
		token_list = token_list->next;
		free(aux_node);
	}
	printf("====================================\n\n");
}

int main(void)
{
	char *cmd = "ls -l";
	t_list *token_lst = make_tokens(cmd);
	print_tokens(token_lst);

	cmd = "cat -e";
	printf("cmd: %s\n", cmd);
	token_lst = make_tokens(cmd);
	print_tokens(token_lst);

	cmd = "ls -l -a -p -m";
	printf("cmd: %s\n", cmd);
	token_lst = make_tokens(cmd);
	print_tokens(token_lst);

	cmd = "ls | cat -e | cat -e | cat -e";
	printf("cmd: %s\n", cmd);
	token_lst = make_tokens(cmd);
	print_tokens(token_lst);

	cmd = "echo \"Hello World\"";
	printf("cmd: %s\n", cmd);
	token_lst = make_tokens(cmd);
	print_tokens(token_lst);

	cmd = "ls|cat -e>outfile";
	printf("cmd: %s\n", cmd);
	token_lst = make_tokens(cmd);
	print_tokens(token_lst);

	cmd = "ls |||";
	printf("cmd: %s\n", cmd);
	token_lst = make_tokens(cmd);
	print_tokens(token_lst);

	cmd = "ls<<<<";
	printf("cmd: %s\n", cmd);
	token_lst = make_tokens(cmd);
	print_tokens(token_lst);

	cmd = "ls<<<<<";
	printf("cmd: %s\n", cmd);
	token_lst = make_tokens(cmd);
	print_tokens(token_lst);

	cmd = "ls<<<<<>>>";
	printf("cmd: %s\n", cmd);
	token_lst = make_tokens(cmd);
	print_tokens(token_lst);

	cmd = "ls<<<<<>>>echo";
	printf("cmd: %s\n", cmd);
	token_lst = make_tokens(cmd);
	print_tokens(token_lst);
}
