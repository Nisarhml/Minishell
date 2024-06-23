/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_to_token_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nihamila <nihamila@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 15:41:24 by nihamila          #+#    #+#             */
/*   Updated: 2024/06/20 17:16:56 by nihamila         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	quotes_in_count_word(char *str, int i, char in_quote)
{
	while (!ft_strchr("\0\"", str[i]) && in_quote == 1)
		i++;
	while (!ft_strchr("\0'", str[i]) && in_quote == 2)
		i++;
	return (i);
}

int	skip_spaces(char *str, int i)
{
	while (is_whitespace(str[i]))
		i++;
	return (i);
}

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
		i = skip_spaces(str, i);
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
			i = quotes_in_count_word(str, i, in_quote);
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
		in_quote = manage_quotes(str[len], in_quote);
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
