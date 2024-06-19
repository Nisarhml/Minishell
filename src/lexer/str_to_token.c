/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_to_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguezzi <aguezzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 09:37:00 by nihamila          #+#    #+#             */
/*   Updated: 2024/06/18 17:57:12 by aguezzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int count_words(char *str)
{
	int		i;
	int		word_count;
	char	in_quote;

	i = 0;
	word_count = 0;
	in_quote = 0;
	while (str[i])
	{
		while (is_whitespace(str[i]))
			i++;
		in_quote = is_in_quote(str[i]);
		if (!in_quote)
		{
			word_count++;
			while (!ft_strchr(" \0\"'><|", str[i]))
				i++;
		}
		else
		{
			i++;
			word_count++;
			while (!ft_strchr("\0\"", str[i]) && in_quote == 1)
				i++;
			while (!ft_strchr("\0'", str[i]) && in_quote == 2)
				i++;
		}
	}
	return (word_count);
}

char	*copy_word_from_str(char *str)
{
	int		i;
	int		len;
	int		in_quote;
	char	*word;

	i = 0;
	len = 0;
	in_quote = 0;
	while (str[len] && (!is_whitespace(str[len]) || in_quote))
	{
		if(str[len] == '"' || str[len] == '\'')
		{
			if(in_quote == 0)
				in_quote = str[len];
			else if (in_quote == str[len])
				in_quote = 0;
		}
		len++;
	}
	word = malloc(len + 1);
	word[len] = '\0';
	while (i < len)
	{
		word[i] = str[i];
		i++;
	}
	return (word);
}

void	fill_word_array(char **words, char *str)
{
	int		i;
	int		word_index;
	char	*word;

	i = 0;
	word_index = 0;
	printf("str vaut : %s\n", str);
	while (str[i])
	{
		while (is_whitespace(str[i]))
			i++;
		if (str[i])
		{
			word = copy_word_from_str(&str[i]);
			words[word_index++] = word;
			while (str[i] && (!is_whitespace(str[i]) || is_in_quote(str[i])))
				i++;
		}
	}
}

t_token *split_string_into_tokens(char *str)
{
	t_token	*lexer;
	char	*word;
	int		i;
	int		j;
	char	in_quote;

	i = 0;
	in_quote = 0;
	lexer = NULL;
	while (str[i])
	{
		while (is_whitespace(str[i]))
			i++;
		word = malloc(ft_strlen(str) + 1);
		j = 0;
		while (str[i] && (!is_whitespace(str[i]) || in_quote))
		{
			if (str[i] == '"' || str[i] == '\'')
			{
				if (in_quote == 0)
					in_quote = str[i];
				else if (in_quote == str[i])
					in_quote = 0;
			}
			else if (is_token(str[i]) && !in_quote)
			{
				if (j > 0)
				{
					word[j] = '\0';
					add_word(&lexer, word, WORD);
					word = malloc(ft_strlen(str) + 1);
					j = 0;
				}
					word[j++] = str[i];
				if (str[i + 1] && str[i + 1] == str[i])
					word[j++] = str[++i];
				word[j] = '\0';
				add_word(&lexer, word, find_token_type(word));
				word = malloc(ft_strlen(str) + 1);
				j = 0;
				i++;
				continue;
			}
			word[j++] = str[i++];
		}
		word[j] = '\0';
		if (word[0] != '\0')
			add_word(&lexer, word, find_token_type(word));
		else
			free(word);
	}
	return (lexer);
}
