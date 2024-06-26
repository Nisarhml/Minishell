/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bloc_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguezzi <aguezzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 21:05:26 by aguezzi           #+#    #+#             */
/*   Updated: 2024/06/26 21:27:56 by aguezzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	process_input(t_begin *begin_list, t_begin_pipes *pipes_list, \
		char *input, char **env)
{
	char	*trimmed_str;
	t_token	*token_list;

	token_list = NULL;
	trimmed_str = trim_input(input);
	if (handle_unclosed_quotes(trimmed_str))
	{
		free(trimmed_str);
		return ;
	}
	token_list = tokenize_and_process(trimmed_str, pipes_list);
	begin_list->first = token_list;
	if (error_pipe_redir(begin_list))
	{
		free(trimmed_str);
		free_lexer(&token_list);
		return ;
	}
	parser_exec(begin_list, pipes_list, env);
	reinit_exec(pipes_list);
	free_list(begin_list);
	free(trimmed_str);
}

int	handle_unclosed_quotes(char *input)
{
	return (unclosed_quotes(input));
}

void	cleanup_and_exit(t_begin *begin_list, t_begin_pipes *pipes_list, \
		t_prompt *prompt_data, int exit_code)
{
	free_all(begin_list, pipes_list, prompt_data);
	exit(exit_code);
}
