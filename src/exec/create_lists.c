/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_lists.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguezzi <aguezzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 11:46:58 by aguezzi           #+#    #+#             */
/*   Updated: 2024/06/26 20:12:02 by aguezzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	init_pipes_list(t_begin_pipes *pipes_list, char **env)
{
	int	i;

	pipes_list->nb_pipes = 0;
	pipes_list->p = NULL;
	pipes_list->pids = NULL;
	pipes_list->pwd = NULL;
	pipes_list->oldpwd = NULL;
	pipes_list->val_dollr = NULL;
	pipes_list->nb_mots = 0;
	pipes_list->sortie_error = 0;
	pipes_list->_stdout = dup(STDOUT_FILENO);
	pipes_list->_stdin = dup(STDIN_FILENO);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PWD=", 4) == 0)
			pipes_list->pwd = ft_strdup(env[i] + 4);
		if (ft_strncmp(env[i], "OLDPWD=", 7) == 0)
			pipes_list->oldpwd = ft_strdup(env[i] + 7);
		i++;
	}
}

t_pipes_part	*init_pipe_part(t_begin_pipes *pipes_list, int i)
{
	t_pipes_part	*pipe_part;

	pipe_part = malloc(sizeof(*pipe_part));
	if (!pipe_part)
		exit(1);
	pipe_part->infile = NULL;
	pipe_part->outfile = NULL;
	pipe_part->cmd = NULL;
	pipe_part->args = NULL;
	pipe_part->path_cmd = NULL;
	pipe_part->if_infile = 0;
	pipe_part->if_heredoc = 0;
	pipe_part->fd[0] = -1;
	pipe_part->fd[1] = -1;
	pipes_list->nb_pipes = 0;
	pipe_part->save_stdout = pipes_list->_stdout;
	pipe_part->next = NULL;
	if (i == 0)
		pipes_list->first = pipe_part;
	return (pipe_part);
}

t_token	*count_and_set_words(t_pipes_part *pipe_part, t_token *token)
{
	t_token	*ref_token;
	int		nb_mots;
	int		j;

	nb_mots = 0;
	ref_token = token;
	while (token && ft_strcmp(token->value, "|") != 0)
	{
		nb_mots++;
		token = token->next;
	}
	pipe_part->words = malloc(sizeof(char *) * (nb_mots + 1));
	if (!pipe_part->words)
		exit(1);
	j = 0;
	while (j < nb_mots)
	{
		pipe_part->words[j] = ref_token->value;
		ref_token = ref_token->next;
		j++;
	}
	pipe_part->words[j] = NULL;
	if (token)
		token = token->next;
	return (token);
}

void	add_pipe_part(t_begin_pipes *pipes_list, t_pipes_part *pipe_part, int i)
{
	t_pipes_part	*current_pipe;

	if (i != 0)
	{
		current_pipe = pipes_list->first;
		while (current_pipe->next)
			current_pipe = current_pipe->next;
		current_pipe->next = pipe_part;
	}
}

void	create_pipes_list(t_begin *begin_list, t_begin_pipes *pipes_list)
{
	t_token			*token;
	t_pipes_part	*pipe_part;
	int				i;

	i = 0;
	token = begin_list->first;
	pipes_list->first = NULL;
	while (token)
	{
		pipe_part = init_pipe_part(pipes_list, i);
		token = count_and_set_words(pipe_part, token);
		add_pipe_part(pipes_list, pipe_part, i);
		i++;
	}
	if (i >= 2)
		pipes_list->nb_pipes = i - 1;
}
