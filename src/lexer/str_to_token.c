/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_to_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nihamila <nihamila@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 09:37:00 by nihamila          #+#    #+#             */
/*   Updated: 2024/06/24 16:59:16 by nihamila         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*allocate_word(char *str)
{
	return malloc(ft_strlen(str) + 1);
}

static void	handle_token(char **word, t_token **lexer, int *j, char *str, int *i)
{
	if (*j > 0)
	{
		(*word)[*j] = '\0';
		add_word(lexer, *word, WORD);
		*word = allocate_word(str);
		*j = 0;
	}
	(*word)[(*j)++] = str[*i];
	if (str[*i + 1] && str[*i + 1] == str[*i])
		(*word)[(*j)++] = str[++(*i)];
	(*word)[*j] = '\0';
	add_word(lexer, *word, find_token_type(*word));
	*word = allocate_word(str);
	*j = 0;
	(*i)++;
}

static void	handle_word(char **word, t_token **lexer, int *j)
{
	(*word)[*j] = '\0';
	if ((*word)[0] != '\0')
		add_word(lexer, *word, find_token_type(*word));
	else
		free(*word);
}

static void	process_string(char *str, t_token **lexer, int *i, char in_quote)
{
	char *word;
	int j;

	j = 0;
	word = allocate_word(str);
	while (str[*i] && (!is_whitespace(str[*i]) || in_quote))
	{
		in_quote = manage_quotes(str[*i], in_quote);
		if (is_token(str[*i]) && !in_quote)
		{
			handle_token(&word, lexer, &j, str, i);
			continue;
		}
		word[j++] = str[(*i)++];
	}
	handle_word(&word, lexer, &j);
}


t_token	*split_string_into_tokens(char *str)
{
	t_token	*lexer;
	int		i;
	char	in_quote;

	i = 0;
	in_quote = 0;
	lexer = NULL;
	while (str[i])
	{
		i = skip_spaces(str, i);
		if (str[i] == '\0')
			break;
		process_string(str, &lexer, &i, in_quote);
	}
	return (lexer);
}
