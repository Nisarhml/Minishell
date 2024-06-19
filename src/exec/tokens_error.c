/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguezzi <aguezzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 15:32:24 by aguezzi           #+#    #+#             */
/*   Updated: 2024/06/18 17:59:25 by aguezzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int redir_error(t_begin_pipes *pipes_list) // je ne gere pas les cas <> et >< , error ou pas ?
{
    t_pipes_part    *pipe_part;
    int             i;
    int             redir;

    pipe_part = malloc(sizeof(*pipe_part));
    if (!pipe_part)
        exit (1);
    pipe_part = pipes_list->first;
    while (pipe_part)
    {
        i = 0;
        redir = -1;
        while (pipe_part->words[i])
        {
            if ((ft_strncmp(pipe_part->words[i], "<", 1) == 0 && ft_strlen(pipe_part->words[i]) > 2)
                || (ft_strncmp(pipe_part->words[i], ">", 1) == 0 && ft_strlen(pipe_part->words[i]) > 2))
                return (1);
            if (ft_strcmp(pipe_part->words[i], "<") == 0 
                || ft_strcmp(pipe_part->words[i], ">") == 0
                || ft_strcmp(pipe_part->words[i], "<<") == 0 
                || ft_strcmp(pipe_part->words[i], ">>") == 0)
            {
                if (redir == i - 1)
                    return (1);
                redir = i;
            }
            i++;
        }
        if (!pipe_part->words[0])
            return (1);
        pipe_part = pipe_part->next;
    }
    return (0);
}

int pipes_error(t_begin_pipes *pipes_list)
{
    t_pipes_part    *pipe_part;
    int             i;
    
    pipe_part = malloc(sizeof(*pipe_part));
    if (!pipe_part)
        exit (1);
    pipe_part = pipes_list->first;
    while (pipe_part)
    {
        if (!pipe_part->words[0])
            return (1);
        i = 0;
        while (pipe_part->words[i])
            i++;
        i--;
        if (ft_strncmp(pipe_part->words[i], "<", 1) == 0 
            || ft_strncmp(pipe_part->words[i], ">", 1) == 0) // verifie si il y a une redir < ou > ou <<<<< ou >> ou... juste avant un pipe
            return (1);
        pipe_part = pipe_part->next;
    }
    return (0);
}

void    ft_error(char *s)
{
    printf("minishell : %s\n", s);
    exit (1);
}