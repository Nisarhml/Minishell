/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nihamila <nihamila@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 11:44:49 by nihamila          #+#    #+#             */
/*   Updated: 2024/06/23 17:14:15 by nihamila         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_valid_envchar(char c)
{
	return ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z')
			|| (c >= 'a' && c <= 'z') || c == '_' || c == '?');
}

int	is_valid_simple_envchar(char c)
{
	return ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z')
			|| (c >= 'a' && c <= 'z') || c == '_' || c == '?');
}

void	remove_char_at(char *str)
{
	int	i;

	i = 0;
	while(str[i])
	{
		str[i] = str[i + 1];
		i++;
	}
}

char env_in_quote(char c, char in_quote)
{
	if (c == '\'' && in_quote == 0)
		return '\'';
	if (c == '\"' && in_quote == 0)
		return '\"';
	if (c == '\'' && in_quote == '\'')
		return 0;
	if (c == '\"' && in_quote == '\"')
		return 0;
	return (in_quote);
}
