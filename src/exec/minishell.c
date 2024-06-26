/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nihamila <nihamila@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 19:21:59 by aguezzi           #+#    #+#             */
/*   Updated: 2024/06/26 14:39:07 by nihamila         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	parser_exec(t_begin *begin_list, t_begin_pipes *pipes_list, char **env)
{
	create_pipes_list(begin_list, pipes_list);
	check_infile_part(pipes_list);
	check_outfile_part(pipes_list);
	check_cmds_args(pipes_list);
	create_heredocs(pipes_list);
	pipes_list->path_env = find_path(pipes_list);
	pipes_list->cmd_paths = ft_split(pipes_list->path_env, ':');
	create_pipes(pipes_list);
	if (pipes_list->nb_pipes >= 1)
	{
		begin_forks(begin_list, pipes_list, env);
		close_pipes_parent(pipes_list);
		wait_childs(pipes_list);
	}
	else
		exec_no_pipe(begin_list, pipes_list, env);
	dup2(pipes_list->_stdin, STDIN_FILENO);
	dup2(pipes_list->_stdout, STDOUT_FILENO);
}

void	exec_no_pipe(t_begin *begin_list, t_begin_pipes *pipes_list, char **env)
{
	t_pipes_part	*pipe_part;
	char			*cmd;
	int				ret;

	pipe_part = pipes_list->first;
	cmd = ft_strdup(pipe_part->cmd);
	ret = 1;
	if (cmd)
	{
		if (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "export") == 0
			|| ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "cd") == 0
			|| ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "env") == 0
			|| ft_strcmp(cmd, "exit") == 0)
			ret = prepa_builtin_solo(pipes_list, pipe_part);
		if (ret)
		{
			if (!builtins(begin_list, pipes_list, pipe_part))
			{
				begin_forks(begin_list, pipes_list, env);
				close_pipes_parent(pipes_list);
				wait_childs(pipes_list);
			}
		}
	}
	free(cmd);
}

int	prepa_builtin_solo(t_begin_pipes *pipes_list, t_pipes_part *pipe_part)
{
	int	i;

	if (!open_close_files(pipe_part))
		return (0);
	define_infile_outfile(pipes_list, pipe_part, 0);
	i = 0;
	while (pipe_part->heredocs[i] != -1)
	{
		if (pipe_part->heredocs[i] != pipe_part->fd[0]
			&& pipe_part->heredocs[i] != pipe_part->fd[1])
			close(pipe_part->heredocs[i]);
		i++;
	}
	if (pipe_part->fd[0] != -1)
	{
		if (dup2(pipe_part->fd[0], STDIN_FILENO) == -1)
			printf("bug dup2 STDIN\n");
	}
	if (pipe_part->fd[1] != -1)
	{
		if (dup2(pipe_part->fd[1], STDOUT_FILENO) == -1)
			printf("bug dup2 STDOUT\n");
	}
	return (1);
}

void	close_pipes_parent(t_begin_pipes *pipes_list)
{
	t_pipes_part	*pipe_part;
	int				i;

	pipe_part = pipes_list->first;
	i = 0;
	while (pipes_list->p[i] != -1)
	{
		close(pipes_list->p[i]);
		i++;
	}
	while (pipe_part)
	{
		i = 0;
		while (pipe_part->heredocs[i] != -1)
		{
			close(pipe_part->heredocs[i]);
			i++;
		}
		pipe_part = pipe_part->next;
	}
}

void	create_pipes(t_begin_pipes *pipes_list)
{
	int	i;

	i = 0;
	pipes_list->p = malloc(sizeof(int) * (pipes_list->nb_pipes * 2 + 1));
	if (!pipes_list->p)
		exit (1);
	while (i < pipes_list->nb_pipes)
	{
		if (pipe(pipes_list->p + (i * 2)) == -1)
			ft_error("Miss create pipe");
		i++;
	}
	pipes_list->p[i * 2] = -1;
}
