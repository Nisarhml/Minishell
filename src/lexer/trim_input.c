/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trim_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nihamila <nihamila@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 11:08:43 by nihamila          #+#    #+#             */
/*   Updated: 2024/06/20 16:42:35 by nihamila         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	manage_quotes(char c, char in_quote)
{
	if (c == '"' || c == '\'')
	{
		if (in_quote == 0)
			in_quote = c;
		else if (in_quote == c)
			in_quote = 0;
	}
	return (in_quote);
}

static	char *extract_word(char *str, int *i)
{
	char *substr;
	char in_quote;
	int start;

	in_quote = 0;
	start = *i;
	while (str[*i] && (!is_whitespace(str[*i]) || in_quote))
	{
		in_quote = manage_quotes(str[*i], in_quote);
		(*i)++;
	}
	substr = ft_substr(str, start, *i - start);
	return (substr);
}

char	*join_words(char *str, char *new_str)
{
	char *tmp;
	char *new_tmp;

	if (!str)
		return (new_str);
	tmp = ft_strjoin(str, " ");
	new_tmp = ft_strjoin(tmp, new_str);
	free(tmp);
	free(new_str);
	free(str);
	return (new_tmp);
}

char	*trim_input(char *str)
{
	char *new_str;
	char *substr;
	int i;

	i = 0;
	new_str = NULL;
	while (str[i])
	{
		i = skip_spaces(str, i);
		if (str[i] == '\0')
			break;
		substr = extract_word(str, &i);
		if (substr && *substr)
			new_str = join_words(new_str, substr);
		else
			free(substr);
	}
	return (new_str);
}


/*char	*trim_input(char *str)
{
	int		start;
	char	in_quote;
	int		i;

	i = 0;
	start = 0;
	while (str[i])
	{
		while (is_whitespace(str[i]))
			i++;
		in_quote = 0;
		while (str[i] && (!is_whitespace(str[i]) || in_quote))
		{
			in_quote = manage_quotes(str[i], in_quote);
			i++;
		}
		join_words(str, ft_substr(str, start, i - start));
	}
	return (str);
}

char	*join_words(char *str, char *new_str)
{
	char *tmp;
	char *new_tmp;
	int start;
	int i;

	i = 0;
	start = 0;
	tmp = NULL;
	new_tmp = NULL;
	if (start < i)
	{
		new_str = ft_substr(str, start, i - start);
		if (!str)
				str = new_str;
		else
		{
			tmp = ft_strjoin(new_str, " ");
			new_tmp = ft_strjoin(tmp, new_str);
			free(tmp);
			free(new_str);
			free(str);
			str = new_tmp;
		}
	}
	return (str);
}*/
