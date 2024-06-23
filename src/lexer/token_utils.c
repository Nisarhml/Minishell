/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nihamila <nihamila@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 14:12:45 by nihamila          #+#    #+#             */
/*   Updated: 2024/06/20 11:08:24 by nihamila         ###   ########.fr       */
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
