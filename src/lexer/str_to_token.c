/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_to_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguezzi <aguezzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 09:37:00 by nihamila          #+#    #+#             */
/*   Updated: 2024/06/26 18:59:10 by aguezzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*allocate_word(char *str)
{
	return (malloc(ft_strlen(str) + 1));
}

static void	handle_token(t_token_data *data)
{
	if (*(data->j) > 0)
	{
		(*(data->word))[*(data->j)] = '\0';
		add_word(data->lexer, *(data->word), WORD);
		*(data->word) = allocate_word(data->str);
		*(data->j) = 0;
	}
	(*(data->word))[(*(data->j))++] = data->str[*(data->i)];
	if (data->str[*(data->i) + 1]
		&& data->str[*(data->i) + 1] == data->str[*(data->i)])
		(*(data->word))[(*(data->j))++] = data->str[++(*(data->i))];
	(*(data->word))[*(data->j)] = '\0';
	add_word(data->lexer, *(data->word), find_token_type(*(data->word)));
	*(data->word) = allocate_word(data->str);
	*(data->j) = 0;
	(*(data->i))++;
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
	char			*word;
	int				j;
	t_token_data	data;

	j = 0;
	word = allocate_word(str);
	data.word = &word;
	data.lexer = lexer;
	data.j = &j;
	data.str = str;
	data.i = i;
	while (str[*i] && (!is_whitespace(str[*i]) || in_quote))
	{
		in_quote = manage_quotes(str[*i], in_quote);
		if (is_token(str[*i]) && !in_quote)
		{
			handle_token(&data);
			continue ;
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
			break ;
		process_string(str, &lexer, &i, in_quote);
	}
	return (lexer);
}
