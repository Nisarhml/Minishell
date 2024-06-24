/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nihamila <nihamila@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 11:26:36 by nihamila          #+#    #+#             */
/*   Updated: 2024/06/24 11:19:05 by nihamila         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	find_token_type(char *str)
{
	if (ft_strlen(str) == 2)
	{
		if (str[0] == '<' && str[1] == '<')
			return (HEREDOC);
		if (str[0] == '>' && str[1] == '>')
			return (APPEND);
	}
	if (ft_strlen(str) == 1)
	{
		if (str[0] == '<')
			return (LESSER);
		if (str[0] == '>')
			return (GREATER);
		if (str[0] == '|')
			return (PIPE);
	}
	return (WORD);
}

void	add_word(t_token **lexer, char *str, int token)
{
	t_token	*new_node;
	t_token	*last;

	new_node = (t_token *)malloc(sizeof(t_token));
	if (!new_node)
		return ;
	new_node->value = str;
	new_node->token = token;
	new_node->next = NULL;
	new_node->prev = NULL;
	if (!*lexer)
		*lexer = new_node;
	else
	{
		last = *lexer;
		while (last->next)
			last = last->next;
		last->next = new_node;
	}
}

t_token	*tokenize_and_process(char *str, t_begin_pipes *pipes_list)
{
	t_token		*lexer;
	t_token		*current;
	char		in_quote;

	in_quote = 0;
	if (ft_strlen(str) == 0)
		return (NULL);
	lexer = split_string_into_tokens(str);
	if (!lexer)
		return (NULL);
	current = lexer;
	while (current)
	{
		if (ft_strchr(current->value, '$'))
			current->value = replace_env_vars(current->value, pipes_list);
		in_quote = remove_unnecessary_quotes(current->value);
		if (in_quote)
		{
			free_lexer(&lexer);
			return (NULL);
		}
		current = current->next;
	}
	return (lexer);
}
