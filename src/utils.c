/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nihamila <nihamila@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 20:45:08 by aguezzi           #+#    #+#             */
/*   Updated: 2024/06/27 11:07:18 by nihamila         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	sp(char *input)
{
	int	i;

	i = 0;
	while (input[i] == ' ')
		i++;
	if (!input[i])
		return (0);
	return (1);
}

int	special_chr_prompt(t_prompt *prompt_data, char *input)
{
	if (ft_strcmp(input, ".") == 0 || ft_strcmp(input, "..") == 0)
	{
		free(prompt_data);
		return (1);
	}
	return (0);
}

void	reinit_exec(t_begin_pipes *pipes_list)
{
	t_pipes_part	*pipe_part;

	if (pipes_list->val_dollr)
	{
		free(pipes_list->val_dollr);
		pipes_list->val_dollr = NULL;
	}
	pipe_part = pipes_list->first;
	free_args_words(pipe_part);
	pipe_part = pipes_list->first;
	while (pipe_part)
	{
		if (pipe_part->type)
			free(pipe_part->type);
		if (pipe_part->infile)
			free(pipe_part->infile);
		if (pipe_part->outfile)
			free(pipe_part->outfile);
		if (pipe_part->path_cmd)
			free(pipe_part->path_cmd);
		if (pipe_part->heredocs)
			free(pipe_part->heredocs);
		pipe_part = pipe_part->next;
	}
	free_pipes_list(pipes_list);
}

t_prompt	*prompt_user_for_input(void)
{
	t_prompt	*prompt_data;

	prompt_data = (t_prompt *)malloc(sizeof(t_prompt));
	if (!prompt_data)
		return (NULL);
	prompt_data->input = readline("MINISHELL <{0_0}> $> ");
	if (prompt_data->input && *prompt_data->input)
		add_history(prompt_data->input);
	return (prompt_data);
}

void	free_args_words(t_pipes_part *pipe_part)
{
	int	i;

	while (pipe_part)
	{
		if (pipe_part->words)
		{
			i = 0;
			while (pipe_part->words[i])
			{
				free(pipe_part->words[i]);
				i++;
			}
			free(pipe_part->words);
		}
		if (pipe_part->args)
			free(pipe_part->args);
		pipe_part = pipe_part->next;
	}
}
