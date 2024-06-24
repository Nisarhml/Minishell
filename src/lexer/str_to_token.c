/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_to_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nihamila <nihamila@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 09:37:00 by nihamila          #+#    #+#             */
/*   Updated: 2024/06/24 11:19:44 by nihamila         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*split_string_into_tokens(char *str)
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
		i = skip_spaces(str, i);
		word = malloc(ft_strlen(str) + 1);
		j = 0;
		while (str[i] && (!is_whitespace(str[i]) || in_quote))
		{
			in_quote = manage_quotes(str[i], in_quote);
			if (is_token(str[i]) && !in_quote)
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
