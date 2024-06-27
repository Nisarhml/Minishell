/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_check_files.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nihamila <nihamila@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 20:55:01 by aguezzi           #+#    #+#             */
/*   Updated: 2024/06/27 12:02:13 by nihamila         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	open_close_files(t_pipes_part *pipe_part)
{
	int	i;

	i = 0;
	while (pipe_part->words[i])
	{
		if (ft_strcmp(pipe_part->words[i], "<") == 0
			&& pipe_part->type[i] != 5)
		{
			if (!open_infile(pipe_part, pipe_part->words[i + 1]))
				return (0);
		}
		else if ((ft_strcmp(pipe_part->words[i], ">") == 0
				|| ft_strcmp(pipe_part->words[i], ">>") == 0)
			&& pipe_part->type[i] != 5)
		{
			if (!open_outfile(pipe_part, pipe_part->words[i + 1], \
				pipe_part->words[i]))
				return (0);
		}
		i++;
	}
	return (1);
}

int	open_outfile(t_pipes_part *pipe_part, char *outfile, char *redir)
{
	if (pipe_part->fd[1] != -1)
		close(pipe_part->fd[1]);
	if (ft_strcmp(redir, ">") == 0)
		pipe_part->fd[1] = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		pipe_part->fd[1] = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (pipe_part->fd[1] < 0)
	{
		if (errno == ENOENT)
			printf("%s: No such file or directory\n", outfile);
		else if (errno == EACCES)
			printf("%s: Permission denied\n", outfile);
		else
			perror("Erreur lors de l'ouverture du fichier\n");
		return (0);
	}
	return (1);
}

int	open_infile(t_pipes_part *pipe_part, char *infile)
{
	if (pipe_part->fd[0] != -1)
		close(pipe_part->fd[0]);
	pipe_part->fd[0] = open(infile, O_RDONLY, 0644);
	if (pipe_part->fd[0] < 0)
	{
		if (errno == ENOENT)
			printf("%s: No such file or directory\n", infile);
		else if (errno == EACCES)
			printf("%s: Permission denied\n", infile);
		else
			perror("Erreur lors de l'ouverture du fichier \n");
		return (0);
	}
	return (1);
}
