/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infile_outfile.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nihamila <nihamila@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 11:19:06 by aguezzi           #+#    #+#             */
/*   Updated: 2024/06/27 12:37:07 by nihamila         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	check_outfile_part(t_begin_pipes *pipes_list)
{
	t_pipes_part	*pipe_part;
	int				i;

	pipe_part = pipes_list->first;
	while (pipe_part)
	{
		i = 0;
		while (pipe_part->words[i])
		{
			if (ft_strcmp(pipe_part->words[i], ">") == 0
				|| ft_strcmp(pipe_part->words[i], ">>") == 0)
			{
				if (pipe_part->outfile)
					free(pipe_part->outfile);
				pipe_part->outfile = ft_strdup(pipe_part->words[i + 1]);
			}
			i++;
		}
		if (!pipe_part->outfile && pipe_part->next)
			pipe_part->outfile = ft_strdup("pipe");
		pipe_part = pipe_part->next;
	}
}

void	check_infile_part(t_begin_pipes *pipes_list)
{
	t_pipes_part	*pipe_part;
	int				i;
	int				tranche_number;

	pipe_part = pipes_list->first;
	tranche_number = 1;
	while (pipe_part)
	{
		i = 0;
		while (pipe_part->words[i])
		{
			if (ft_strcmp(pipe_part->words[i], "<") == 0
				&& pipe_part->type[i] != 5)
				bloc_redir_in(pipe_part, i);
			else if (ft_strcmp(pipe_part->words[i], "<<") == 0
				&& pipe_part->type[i] != 5)
				pipe_part->if_heredoc = 1;
			i++;
		}
		if (!pipe_part->infile && tranche_number != 1)
			pipe_part->infile = ft_strdup("pipe");
		tranche_number++;
		pipe_part = pipe_part->next;
	}
}

void	bloc_redir_in(t_pipes_part *pipe_part, int i)
{
	if (pipe_part->infile)
		free(pipe_part->infile);
	pipe_part->infile = ft_strdup(pipe_part->words[i + 1]);
	pipe_part->if_infile = 1;
	pipe_part->if_heredoc = 0;
}
