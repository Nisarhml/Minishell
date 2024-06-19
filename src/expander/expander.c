/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguezzi <aguezzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 12:30:21 by nihamila          #+#    #+#             */
/*   Updated: 2024/06/18 17:56:34 by aguezzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_env_name(char *str)
{
	char	*name;
	int		len;
	int		i;

	len = 0;
	i = 0;
	while(is_valid_simple_envchar(str[len]))
		len++;
	name = ft_calloc(sizeof(char), len + 1);
	while(i < len)
	{
		name[i] = str[i];
		i++;
	}
	return (name);
}

char	*replace_env_var(char *str, int i)
{
	char	*name;
	char	*value;
	char	*new_str;

	name = get_env_name(str + i);
	value = getenv(name);
	str[i - 1] = '\0';
	while(is_valid_simple_envchar(str[i]))
		i++;
	if (!value)
		value = ft_strdup("");
	new_str = ft_strjoin_three_parts(str, value,str + i);
	free(name);
	free(str);
	return (new_str);
}

char	*replace_env_vars(char *str)
{
	int 	i;
	char	in_quote;

	i = 0;
	in_quote = 0;
	while(str && str[i])
	{
		in_quote = env_in_quote(str[i], in_quote);
		if(str[i] == '$' && in_quote != '\'')
		{
				printf("str = %s\n str[%c]\n", str, str[i]);
				//printf("str[%c]\n", str[i]);
				str = replace_env_var(str, i + 1);
		}
		i++;
	}
	return (str);
}

void	remove_char_at(char *str)
{
	int	i;

	i = 0;
	while(str[i])
	{
		str[i] = str[i + 1];
		i++;
		printf("str[%c]\n", str[i]);
	}
}

char	remove_unnecessary_quotes(char *str)
{
	int		i;
	char	in_quote;

	i = 0;
	in_quote = 0;
	while (str && str[i])
	{
		in_quote = env_in_quote(str[i], in_quote);
		if (!in_quote)
			i++;
		else
		{
			remove_char_at(str + i);
			while (in_quote && str[i])
			{
				in_quote = env_in_quote(str[i], in_quote);
				if (in_quote)
					i++;
			}
			remove_char_at(str + i);
		}
	}
	return (in_quote);
}
/*test*/
