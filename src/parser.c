/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 13:34:26 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/05 19:39:28 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_list	*make_tokens(char *user_input);

t_command *create_expression(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
		return (NULL);
	cmd->has_pipe = 0;
	cmd->has_redirection = 0;
	cmd->words = NULL;
	return (cmd);
}

t_token *get_next_token(t_list *token_list)
{
	t_token *tkn;

	tkn = NULL;
	if (token_list->next != NULL)
	{
		tkn = (t_token *) token_list->next->content;
	}
	return (tkn);
}

int	pipe_rules(t_token *prev_tkn, t_token *curr_token, t_token *next_tkn)
{
	int		syntax;

	syntax = 0;
	if (curr_token->type == PIPE)
	{
		if (!(prev_tkn != NULL && next_tkn != NULL))
		{
			syntax = -1;
		}
		else if ((prev_tkn->type == WORD && (next_tkn->type == WORD || next_tkn->type == REDIRECT)))
		{
			syntax = 1;
		}
		else
		{
			syntax = -1;
		}
	}
	else
		return (syntax);
	if (syntax == -1)
		printf("syntax error near '|'\n");
	return (syntax);
}

int	redirect_rules(t_token *prev_tkn, t_token *curr_token, t_token *next_tkn)
{
	int	syntax;

	syntax = 0;
	if (curr_token->type == REDIRECT)
	{
		if (!next_tkn)
			syntax = -1;
		else if (next_tkn->type == WORD)
			syntax = 1;
		else
			syntax = -1;
	}	
	else
		return (syntax);
	if (syntax == -1)
		printf("syntax error near '<' or '>' \n");
	return (syntax);
}

int	check_syntax(t_list *token_list)
{
	t_list	*node;
	t_token	*tkn;
	t_token	*prev_tkn;
	t_token *next_tkn;

	node = token_list;
	prev_tkn = NULL;
	next_tkn = NULL;
	while (token_list)
	{
		tkn = (t_token *) token_list->content;
		next_tkn = get_next_token(token_list);
		if (pipe_rules(prev_tkn, tkn, next_tkn) == -1) 
			return (0);
		if (redirect_rules(prev_tkn, tkn, next_tkn) == -1)
			return (0);
		prev_tkn = tkn;
		token_list = token_list->next;
	}
	return (1);
}

/*t_command	*parse_expression(t_list *token_list)
{
	t_list		*node;
	t_token		*tkn;
	t_command	*cmd;
	char		**tokens;

	return (NULL);
}*/

void	print_syntax_ok(char *cmd, int expected)
{
	t_list *token_list;
	int		syntax_ok;

	token_list = make_tokens(cmd);
	syntax_ok = check_syntax(token_list);
	printf("cmd: %s\n", cmd);
	printf("expected: %d, got: %d\n", expected, syntax_ok);
	printf("result: ");
	if (syntax_ok == expected)
	{
		printf("\033[0;32m");
		printf("OK\n");
		printf("\033[0m"); 
	}
	else
	{
		printf("\033[0;31m");
		printf("KO\n");
		printf("\033[0m"); 
	}
	printf("+++\n");	
}

#define FAIL 0 
#define OK 1 

int main(void)
{
	char *cmd;

	cmd = "ls -l";
	print_syntax_ok(cmd, OK);

	cmd = "ls |||";
	print_syntax_ok(cmd, FAIL);

	cmd = "ls |cat -e|";
	print_syntax_ok(cmd, FAIL);

	cmd = "ls |cat -e| echo";
	print_syntax_ok(cmd, OK);

	printf("\n======= '<' SYNTAX CHECK =======\n");
	cmd = "<";
	print_syntax_ok(cmd, FAIL);

	cmd = "< ls";
	print_syntax_ok(cmd, OK);

	cmd = "<ls";
	print_syntax_ok(cmd, OK);

	cmd = "cat -e < infile";
	print_syntax_ok(cmd, OK);

	cmd = "cat -e<infile";
	print_syntax_ok(cmd, OK);

	cmd = "< infile cat -e";
	print_syntax_ok(cmd, OK);

	cmd = "cat -e <";
	print_syntax_ok(cmd, FAIL);

	cmd = "cat -e < <";
	print_syntax_ok(cmd, FAIL);

	cmd = "cat -e < < <";
	print_syntax_ok(cmd, FAIL);

	cmd = "< < cat -e";
	print_syntax_ok(cmd, FAIL);

	printf("\n======= '>' SYNTAX CHECK =======\n");
	cmd = ">";
	print_syntax_ok(cmd, FAIL);

	cmd = "> ls";
	print_syntax_ok(cmd, OK);

	cmd = "> > ls";
	print_syntax_ok(cmd, FAIL);

	cmd = "ls >";
	print_syntax_ok(cmd, FAIL);

	cmd = "> ls >";
	print_syntax_ok(cmd, FAIL);

	cmd = "ls > outfile";
	print_syntax_ok(cmd, OK);

	cmd = "ls > outfile > outfile2";
	print_syntax_ok(cmd, OK);

	cmd = "ls > outfile > outfile2 > outfile3";
	print_syntax_ok(cmd, OK);

	cmd = "ls>outfile>outfile2>outfile3";
	print_syntax_ok(cmd, OK);

	cmd = "ls>outfile>outfile2>";
	print_syntax_ok(cmd, FAIL);

	cmd = "ls> >";
	print_syntax_ok(cmd, FAIL);

	cmd = "ls>cat";
	print_syntax_ok(cmd, OK);

	printf("\n======= '>>' SYNTAX CHECK =======\n");
	cmd = ">>";
	print_syntax_ok(cmd, FAIL);

	cmd = "ls>>cat";
	print_syntax_ok(cmd, OK);

	cmd = "ls >> cat";
	print_syntax_ok(cmd, OK);

	cmd = "ls >>";
	print_syntax_ok(cmd, FAIL);

	cmd = "ls >>>>";
	print_syntax_ok(cmd, FAIL);

	cmd = "ls >> cat >>";
	print_syntax_ok(cmd, FAIL);

	cmd = "ls>>cat>>";
	print_syntax_ok(cmd, FAIL);

	cmd = "ls >> cat >> echo";
	print_syntax_ok(cmd, OK);

	cmd = "ls>>cat>>echo";
	print_syntax_ok(cmd, OK);

	cmd = ">> ls";
	print_syntax_ok(cmd, OK);
	cmd = ">>ls";
	print_syntax_ok(cmd, OK);

	printf("\n======= '<<' SYNTAX CHECK =======\n");
	cmd = "cat <<";
	print_syntax_ok(cmd, FAIL);

	cmd = "cat << -";
	print_syntax_ok(cmd, OK);

	cmd = "<< cat -e";
	print_syntax_ok(cmd, OK);

	cmd = "<<";
	print_syntax_ok(cmd, FAIL);

	printf("\n======= MISC SYNTAX CHECK =======\n");
	cmd = "ls | cat -e > outfile";
	print_syntax_ok(cmd, OK);

	cmd = "cat | cat -e > outfile | cat -e";
	print_syntax_ok(cmd, OK);

	cmd = "cat -e << - | rev | >";
	print_syntax_ok(cmd, FAIL);

	cmd = "cat -e << - | rev | > infile";
	print_syntax_ok(cmd, OK);

	cmd = "cat << >give";
	print_syntax_ok(cmd, FAIL);

	cmd = "ls <<>> rev";
	print_syntax_ok(cmd, FAIL);

	cmd = "ls | rev | < infile";
	print_syntax_ok(cmd, OK);
}

