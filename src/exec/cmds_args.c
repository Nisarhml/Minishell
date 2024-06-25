/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguezzi <aguezzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 11:20:17 by aguezzi           #+#    #+#             */
/*   Updated: 2024/06/25 23:19:46 by aguezzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void    check_cmds_args(t_begin_pipes *pipes_list)
{
    t_pipes_part    *pipe_part;
    int             i;
    
    pipe_part = pipes_list->first;
    while (pipe_part)
    {
        i = 0;
        pipe_part->nb_arg = 0;
        while (pipe_part->words[i])
        {
            check_if_redir(pipe_part, &pipe_part->nb_arg, &i);
            i++;
        }
        create_part_args(pipe_part);
        pipe_part = pipe_part->next;
    }
}

void    check_if_redir(t_pipes_part *pipe_part, int *nb_arg, int *i)
{
    if (ft_strcmp(pipe_part->words[*i], ">") != 0
        && ft_strcmp(pipe_part->words[*i], ">>") != 0
        && ft_strcmp(pipe_part->words[*i], "<") != 0
        && ft_strcmp(pipe_part->words[*i], "<<") != 0)
    {
        if (!pipe_part->cmd)
            pipe_part->cmd = pipe_part->words[*i];
        else
            (*nb_arg)++;
    }
    else
        (*i)++;
}

void    create_part_args(t_pipes_part *pipe_part)
{
    int i;
    int j;

    i = 0;
    j = 0;
    pipe_part->args = malloc(sizeof(char *) * (pipe_part->nb_arg + 2));
    if (!pipe_part->args)
        exit (1);
    while (pipe_part->words[i])
    {
        if (ft_strcmp(pipe_part->words[i], ">") != 0
            && ft_strcmp(pipe_part->words[i], ">>") != 0
            && ft_strcmp(pipe_part->words[i], "<") != 0
            && ft_strcmp(pipe_part->words[i], "<<") != 0)
        {
            pipe_part->args[j] = pipe_part->words[i];
            j++;
        }
        else
            i++;
        i++;
    }
    pipe_part->args[j] = NULL;
}