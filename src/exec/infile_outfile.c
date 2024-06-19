/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infile_outfile.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguezzi <aguezzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 11:19:06 by aguezzi           #+#    #+#             */
/*   Updated: 2024/06/18 17:59:10 by aguezzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"  // on pourrait delete ce fichier peut etre bien plus tard (permet juste de debugger au debut)

void    affich_infiles_outfiles(t_begin_pipes *pipes_list)
{
    t_pipes_part    *pipe_part;
    int             i;

    pipe_part = pipes_list->first;
    i = 1;
    while (pipe_part)
    {
        if (!pipe_part->infile)
            printf("tranche %d : infile = default ; ", i);
        else
            printf("tranche %d : infile = %s ; ", i, pipe_part->infile);
        if (!pipe_part->outfile)
            printf("outfile = default\n");
        else
            printf("outfile = %s\n", pipe_part->outfile);
        pipe_part = pipe_part->next;
        i++;
    }
    printf("\n");
}

void    check_outfile_part(t_begin_pipes *pipes_list)
{
    t_pipes_part    *pipe_part;
    int             i;

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

void    check_infile_part(t_begin_pipes *pipes_list)
{
    t_pipes_part    *pipe_part;
    int             i;
    int             tranche_number;

    pipe_part = pipes_list->first;
    tranche_number = 1;
    while (pipe_part)
    {
        i = 0;
        while (pipe_part->words[i])
        {
            if (ft_strcmp(pipe_part->words[i], "<") == 0)
            {
                if (pipe_part->infile)
                    free(pipe_part->infile);
                pipe_part->infile = ft_strdup(pipe_part->words[i + 1]);
                pipe_part->if_infile = 1;
                pipe_part->if_heredoc = 0;
            }
            else if (ft_strcmp(pipe_part->words[i], "<<") == 0)
            {
                pipe_part->if_infile = 0;
                pipe_part->if_heredoc = 1;
            }
            i++;
        }
        if (!pipe_part->infile && tranche_number != 1)
            pipe_part->infile = ft_strdup("pipe");
        tranche_number++;
        pipe_part = pipe_part->next;
    }
}