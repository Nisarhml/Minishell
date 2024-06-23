/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nihamila <nihamila@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 11:54:50 by nihamila          #+#    #+#             */
/*   Updated: 2024/06/19 18:34:01 by nihamila         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h" //norm done !

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
	if (word_count <= 0)
		return (NULL);
	words = malloc(sizeof(char *) * (word_count + 1));
	if (!words)
		return (NULL);
	fill_word_array(words, str);
	words[word_count] = NULL;
	return (words);
}

void	check_d_quote(char c, int *in_d_quote, int *in_s_quote, int *d_q_count)
{
	if (c == '"' && !(*in_s_quote))
	{
		*in_d_quote = !(*in_d_quote);
		if (*in_d_quote)
			(*d_q_count)++;
	}
}

void	check_s_quote(char c, int *in_d_quote, int *in_s_quote, int *s_q_count)
{
	if (c == '\'' && !(*in_d_quote))
	{
		*in_s_quote = !(*in_s_quote);
		if (*in_s_quote)
			(*s_q_count)++;
	}
}

int	unclosed_quotes(char *str)
{
	int	s_quote_count;
	int	d_quote_count;
	int	is_in_d_quote;
	int	is_in_s_quote;
	int	i;

	i = 0;
	s_quote_count = 0;
	d_quote_count = 0;
	is_in_d_quote = 0;
	is_in_s_quote = 0;
	while (str[i])
	{
		check_d_quote(str[i], &is_in_d_quote, &is_in_s_quote, &d_quote_count);
		check_s_quote(str[i], &is_in_d_quote, &is_in_s_quote, &s_quote_count);
		i++;
	}
	if (is_in_d_quote || is_in_s_quote)
	{
		printf("Error: quote unclosed !\n");
		return (1);
	}
	return (0);
}
