/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguezzi <aguezzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 14:12:45 by nihamila          #+#    #+#             */
/*   Updated: 2024/06/18 17:57:17 by aguezzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int is_token(char c)
{
	return (c == '|' || c == '<' || c == '>');
}
int is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r');
}
char	*ft_strjoin_free(char *s1, char *s2, int to_free)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	if (to_free == 1 || to_free == 3)
		free(s1);
	if (to_free == 2 || to_free == 3)
		free(s2);
	return (result);
}

char *trim_input(char *str)
{
	char 	*new_str;
	char	*substr;
	int		start;
	char	in_quote;
	int		i;

	i = 0;
	new_str = NULL;
	substr = NULL;
	while (str[i])
	{
		while (is_whitespace(str[i]))
			i++;
		start = i;
		in_quote = 0;
		while (str[i] && (!is_whitespace(str[i]) || in_quote))
		{
			if (str[i] == '"' || str[i] == '\'')
			{
				if (in_quote == 0)
					in_quote = str[i];
				else if (in_quote == str[i])
					in_quote = 0;
			}
			i++;
		}
		if (start < i)
		{
			substr = ft_substr(str, start, i - start);
			if (!new_str)
				new_str = substr;
			else
			{
				char *tmp = ft_strjoin(new_str, " ");
				char *new_tmp = ft_strjoin(tmp, substr);
				free(tmp);
				free(substr);
				free(new_str);
				new_str = new_tmp;
			}
		}
	}
	return (new_str);
}
