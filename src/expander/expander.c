/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nihamila <nihamila@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 12:30:21 by nihamila          #+#    #+#             */
/*   Updated: 2024/06/25 11:52:41 by nihamila         ###   ########.fr       */
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
	while (is_valid_simple_envchar(str[len]))
		len++;
	name = ft_calloc(sizeof(char), len + 1);
	while (i < len)
	{
		name[i] = str[i];
		i++;
	}
	return (name);
}

char	*get_env_value(t_begin_pipes *pipes_list, char *name)
{
	t_var_env	*current;

	current = pipes_list->env_list->first;
	while (current)
	{
		printf("ft_strcmp(current->name, name) = %d\n", ft_strcmp(current->name, name));
		printf("current->value = %s\n", current->value);
		if (ft_strcmp(current->name, name) == 0)
			return (current->value);
		current = current->next;
		printf("current = %s\n", current->name);
	}
	return (NULL);
}

char	*replace_env_var(char *str, int i, t_begin_pipes *pipes_list)
{
	char	*name;
	char	*value;
	char	*new_str;

	name = get_env_name(str + i);
	printf("name = %s\n", name);
	if (ft_strcmp(name, "?") == 0)
		value = ft_itoa(pipes_list->sortie_error);
	else
	{
		value = get_env_value(pipes_list, name);
		printf("value = %s\n", value);
	}
	str[i - 1] = '\0';
	while (is_valid_simple_envchar(str[i]))
		i++;
	if (!value)
		value = ft_strdup("");
	new_str = ft_strjoin_three_part(str, value, str + i);
	free(name);
	free(str);
	return (new_str);
}

char	*replace_env_vars(char *str, t_begin_pipes *pipes_list)
{
	int		i;
	char	in_quote;

	i = 0;
	in_quote = 0;
	while (str && str[i])
	{
		in_quote = env_in_quote(str[i], in_quote);
		if (str[i] == '$' && in_quote != '\'' && str[i + 1]
			&& is_valid_simple_envchar(str[i + 1]))
			str = replace_env_var(str, i + 1, pipes_list);
		i++;
	}
	return (str);
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
