/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguezzi <aguezzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 11:54:50 by nihamila          #+#    #+#             */
/*   Updated: 2024/06/18 20:01:21 by aguezzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_in_quote(char c)
{
	if (c == '"')
		return (1);
	else if (c == '\'')
		return (2);
	return (0);
}

char	**split_str_by_spaces_and_quotes(char *str)
{
	int		word_count;
	char	**words;

	word_count = count_words(str);
	words = NULL;
	if(word_count <= 0)
		return (NULL);
	words = malloc(sizeof(char *) * (word_count + 1));
	if (!words)
		return (NULL);
	fill_word_array(words, str);
	words[word_count] = NULL;
	return (words);
}

int	unclosed_quotes(char *str)
{
	int single_quote_count;
	int double_quote_count;
	int	is_in_d_quote;
	int	is_in_s_quote;
	int i;

	i = 0;
	single_quote_count = 0;
	double_quote_count = 0;
	is_in_s_quote = 0;
	is_in_d_quote = 0;
	while (str[i])
	{
		if (str[i] == '"')
		{
			if (!is_in_s_quote)
			{
				is_in_d_quote = !is_in_d_quote;
				if (is_in_d_quote)
					double_quote_count++;
			}
		}
		else if (str[i] == '\'')
		{
			if (!is_in_d_quote)
			{
				is_in_s_quote = !is_in_s_quote;
				if (is_in_s_quote)
					single_quote_count++;
			}
		}
		i++;
	}
	if (is_in_d_quote || is_in_s_quote)
	{
		printf("Error: quote unclosed !\n");
		return (1);
	}
	return (0);
}

