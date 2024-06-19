/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_part.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguezzi <aguezzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 16:00:42 by aguezzi           #+#    #+#             */
/*   Updated: 2024/06/19 13:05:14 by aguezzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void    begin_forks(t_begin_pipes *pipes_list, char **env)
{
    t_pipes_part    *pipe_part;
    int             i;
    
    pipes_list->pids = malloc(sizeof(pid_t) * (pipes_list->nb_pipes + 1));
    if (!pipes_list->pids)
        exit (1);
    pipe_part = pipes_list->first;
    i = 0;
    while(i < pipes_list->nb_pipes + 1)
    {
        pipes_list->pids[i] = fork();
        if (pipes_list->pids[i] == -1)
            ft_error("Miss create fork");
        child_process(pipes_list, pipe_part, env, i);
        pipe_part = pipe_part->next;
        i++;
    }
}

void    child_process(t_begin_pipes *pipes_list, t_pipes_part *pipe_part, char **env, int i)
{
    if (pipes_list->pids[i] == 0)
    {
        if (!open_close_files(pipe_part))
            exit (1);
        pipe_part->path_cmd = search_the_path(pipes_list, pipe_part);
        define_infile_outfile(pipes_list, pipe_part, i);
        close_pipes_child(pipes_list, pipe_part);
        if (pipe_part->fd[0] != -1)
        {
            if (dup2(pipe_part->fd[0], STDIN_FILENO) == -1)
                printf("error de dup2 avec fd = %d en STDIN\n", pipe_part->fd[0]);
            close(pipe_part->fd[0]);
        }
        if (pipe_part->fd[1] != -1)
        {
            if (dup2(pipe_part->fd[1], STDOUT_FILENO) == -1)
                printf("error de dup2 avec fd = %d en STDOUT\n", pipe_part->fd[1]);
            close(pipe_part->fd[1]);
        }
        if (builtins(pipes_list, pipe_part))
            exit (0);
        execve(pipe_part->path_cmd, pipe_part->args, env);
    }
}

void	wait_childs(t_begin_pipes *pipes_list)
{
	int	i;

	i = 0;
	while (i < pipes_list->nb_pipes + 1)
	{
		waitpid(pipes_list->pids[i], NULL, 0);
		i++;
	}
}

void    close_pipes_child(t_begin_pipes *pipes_list, t_pipes_part *pipe_part)
{
    int i;
    
    i = 0;
    while (pipe_part->heredocs[i] != -2)
    {
        if(pipe_part->heredocs[i] != pipe_part->fd[0]
            && pipe_part->heredocs[i] != pipe_part->fd[1])
        close(pipe_part->heredocs[i]);
        i++;
    }
    i = 0;
    while (pipes_list->p[i] != -1)
    {
        if(pipes_list->p[i] != pipe_part->fd[0]
            && pipes_list->p[i] != pipe_part->fd[1])
        close(pipes_list->p[i]);
        i++;
    }
}

void    define_infile_outfile(t_begin_pipes *pipes_list, t_pipes_part *pipe_part, int i)
{
    if (pipe_part->if_heredoc) // je regarde si mon infile doit etre un heredoc
    {
        if (pipe_part->fd[0] != -1)
            close(pipe_part->fd[0]);
        pipe_part->fd[0] = last_heredoc(pipe_part);
    }
    else if (pipe_part->fd[0] == -1 && i != 0) // sinon mon infile devrait etre un pipe si je ne suis pas sur le 1er segment
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

int last_heredoc(t_pipes_part *pipe_part)
{
    int i;

    i = 0;
    while (pipe_part->heredocs[i] != -2)
        i++;
    return (pipe_part->heredocs[i - 2]);
}