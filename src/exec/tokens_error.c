/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nihamila <nihamila@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 15:32:24 by aguezzi           #+#    #+#             */
/*   Updated: 2024/06/26 14:59:13 by nihamila         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	error_pipe_redir(t_begin *begin_list)
{
	t_token	*current;

	current = begin_list->first;
	if (current->value[0] == '|')
	{
		printf("syntax error near unexpected token `|'\n");
		return (1);
	}
	while (current)
	{
		if (bloc1_err(current))
			return (1);
		if (bloc2_err(current))
			return (1);
		current = current->next;
	}
	return (0);
}

int	bloc1_err(t_token *current)
{
	if (ft_strcmp(current->value, "||") == 0)
	{
		printf("syntax error near unexpected token `|'\n");
		return (1);
	}
	if (current->token == 4)
	{
		if (current->next)
		{
			if (current->next->token == 4)
			{
				printf("syntax error near unexpected token ");
				printf("`%c`\n", current->next->value[0]);
				return (1);
			}
		}
	}
	return (0);
}

int	bloc2_err(t_token *current)
{
	if (current->token == 0 || current->token == 1
		|| current->token == 2 || current->token == 3)
	{
		if (current->next)
		{
			if (current->next->token != 5)
			{
				printf("syntax error near unexpected token ");
				printf("`%c`\n", current->next->value[0]);
				return (1);
			}
		}
		else
		{
			printf("syntax error near unexpected token `newline`\n");
			return (1);
		}
	}
	return (0);
}

void	ft_error(char *s)
{
	printf("minishell : %s\n", s);
	exit (127);
}
