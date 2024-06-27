/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_part.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nihamila <nihamila@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 16:00:42 by aguezzi           #+#    #+#             */
/*   Updated: 2024/06/27 11:37:42 by nihamila         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	begin_forks(t_begin *begin_list, t_begin_pipes *pipes_list, char **env)
{
	t_pipes_part	*pipe_part;

	pipes_list->pids = malloc(sizeof(pid_t) * (pipes_list->nb_pipes + 1));
	if (!pipes_list->pids)
		exit (1);
	pipe_part = pipes_list->first;
	pipes_list->i = 0;
	while (pipes_list->i < pipes_list->nb_pipes + 1)
	{
		pipes_list->pids[pipes_list->i] = fork();
		if (pipes_list->pids[pipes_list->i] == -1)
			ft_error("Miss create fork");
		child_process(begin_list, pipes_list, pipe_part, env);
		pipe_part = pipe_part->next;
		(pipes_list->i)++;
	}
}

void	child_process(t_begin *begin_list, t_begin_pipes *pipes_list, \
						t_pipes_part *pipe_part, char **env)
{
	if (pipes_list->pids[pipes_list->i] == 0)
	{
		set_command_signals();
		if (!open_close_files(pipe_part))
			exit (1);
		pipe_part->path_cmd = search_the_path(pipes_list, pipe_part);
		define_infile_outfile(pipes_list, pipe_part, pipes_list->i);
		close_pipes_child(pipes_list, pipe_part);
		if (pipe_part->fd[0] != -1)
		{
			if (dup2(pipe_part->fd[0], STDIN_FILENO) == -1)
				printf("error de dup2 avec fd = %d en STDIN\n", \
					pipe_part->fd[0]);
		}
		if (pipe_part->fd[1] != -1)
		{
			if (dup2(pipe_part->fd[1], STDOUT_FILENO) == -1)
				printf("error de dup2 avec fd = %d en STDOUT\n", \
					pipe_part->fd[1]);
		}
		if (builtins(begin_list, pipes_list, pipe_part))
			exit (0);
		execve(pipe_part->path_cmd, pipe_part->args, env);
	}
	ignore_signals();
}

void	wait_childs(t_begin_pipes *pipes_list)
{
	int	i;
	int	status;

	i = 0;
	while (i < pipes_list->nb_pipes + 1)
	{
		waitpid(pipes_list->pids[i], &status, 0);
		set_basic_signals();
		if (WIFEXITED(status))
			set_exit_status(WEXITSTATUS(status));
		else if (WIFSIGNALED(status))
		{
			set_exit_status(128 + WTERMSIG(status));
			if (WTERMSIG(status) == 2)
				printf("\n");
			if (WTERMSIG(status) == 3)
				printf("Quit (core dumped)\n");
		}
		else
			set_exit_status(127);
		i++;
	}
}

void	close_pipes_child(t_begin_pipes *pipes_list, t_pipes_part *pipe_part)
{
	int	i;

	i = 0;
	while (pipe_part->heredocs[i] != -1)
	{
		if (pipe_part->heredocs[i] != pipe_part->fd[0]
			&& pipe_part->heredocs[i] != pipe_part->fd[1])
			close(pipe_part->heredocs[i]);
		i++;
	}
	i = 0;
	while (pipes_list->p[i] != -1)
	{
		if (pipes_list->p[i] != pipe_part->fd[0]
			&& pipes_list->p[i] != pipe_part->fd[1])
			close(pipes_list->p[i]);
		i++;
	}
}

void	define_infile_outfile(t_begin_pipes *pipes_list, \
		t_pipes_part *pipe_part, int i)
{
	if (pipe_part->if_heredoc)
	{
		if (pipe_part->fd[0] != -1)
			close(pipe_part->fd[0]);
		pipe_part->fd[0] = last_heredoc(pipe_part);
	}
	else if (pipe_part->fd[0] == -1 && i != 0)
	{
		if (pipe_part->fd[0] != -1)
			close(pipe_part->fd[0]);
		pipe_part->fd[0] = pipes_list->p[i * 2 - 2];
	}
	if (pipe_part->fd[1] == -1)
	{
		if (pipe_part->next)
			pipe_part->fd[1] = pipes_list->p[i * 2 + 1];
	}
}
