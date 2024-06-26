/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguezzi <aguezzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 15:57:14 by nihamila          #+#    #+#             */
/*   Updated: 2024/06/26 18:26:00 by aguezzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	contain_token(char *str)
{
	int		i;
	char	in_quote;

	i = 0;
	in_quote = 0;
	while (str[i])
	{
		is_in_quote(str[i]);
		if (is_token(str[i]) && !in_quote)
			return (1);
		i++;
	}
	return (0);
}

char	*get_word(char *str)
{
	int		i;
	char	in_quote;
	char	*word;

	i = 0;
	in_quote = 0;
	while (str[i] && (!is_token(str[i]) || in_quote))
	{
		is_in_quote(str[i]);
		i++;
	}
	word = ft_calloc(sizeof(char), i + 1);
	i = 0;
	in_quote = 0;
	is_in_quote(str[i]);
	while (str[i] && (!is_token(str[i]) || in_quote))
	{
		word[i] = str[i];
		is_in_quote(str[i]);
		i++;
	}
	word[i] = '\0';
	return (word);
}

void	free_lexer(t_token **lexer)
{
	t_token	*tmp;
	t_token	*current;

	current = *lexer;
	while (current)
	{
		tmp = current;
		current = current->next;
		free(tmp->value);
		free(tmp);
	}
}
