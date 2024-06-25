/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_lists.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguezzi <aguezzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 11:46:58 by aguezzi           #+#    #+#             */
/*   Updated: 2024/06/26 00:22:21 by aguezzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void    init_pipes_list(t_begin_pipes *pipes_list, char **env)
{
    int i;
    
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

void    create_pipes_list(t_begin *begin_list, t_begin_pipes *pipes_list)
{
    t_pipes_part    *pipe_part;
    t_pipes_part    *current_pipe;
    t_token         *token;
    t_token         *ref_token; // va permettre de savoir ou je me suis arrete pour reprendre a stocker mes char *value
    int             i; // correspond a l'index de ma pipes_list
    int             j; // stocker les char *value avec le bon index j
    int             nb_mots;

    pipes_list->first = NULL;
    token = begin_list->first;
    i = 0;
    while (token)
    {
        pipe_part = malloc(sizeof(*pipe_part));
        if (!pipe_part)
            exit (1);
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
        if (i == 0)
            pipes_list->first = pipe_part;
        nb_mots = 0;
        ref_token = token;
        while (token && ft_strcmp(token->value, "|") != 0)
        {
            nb_mots++;
            token = token->next;
        }
        pipe_part->words = malloc(sizeof(char *) * (nb_mots + 1));
        if (!pipe_part->words)
            exit (1);
        j = 0;
        while (j < nb_mots)
        {
            pipe_part->words[j] = ref_token->value;
            ref_token = ref_token->next;
            j++;
        }
        pipe_part->words[j] = NULL;
        if (i != 0)
        {
            current_pipe = pipes_list->first;
            while (current_pipe->next)
                current_pipe = current_pipe->next;
            current_pipe->next = pipe_part;
        }
        pipe_part->next = NULL;
        if (token)
            token = token->next;
        i++;
    }
    if (i >= 2)
        pipes_list->nb_pipes = i - 1;
}

/*void    create_pipes_list(t_begin *begin_list, t_begin_pipes *pipes_list)
{
    t_pipes_part    *pipe_part;
    t_token         *token;
    t_token         *ref_token; // va permettre de savoir ou je me suis arrete pour reprendre a stocker mes char *value
    int             i; // correspond a l'index de ma pipes_list

    pipes_list->first = NULL;
    token = begin_list->first;
    ref_token = token;
    i = 0;
    while (token)
    {
        pipe_part = malloc(sizeof(*pipe_part));
        if (!pipe_part)
            exit (1);
        bloc1_pip_list(pipes_list, pipe_part, token, ref_token, i);
        printf("mots = %d\n", pipes_list->nb_mots);
        pipe_part->words = malloc(sizeof(char *) * (pipes_list->nb_mots + 1));
        if (!pipe_part->words)
            exit (1);
        bloc2_pip_list(pipes_list, pipe_part, ref_token, i);
        if (token)
            token = token->next;
        i++;
    }
    if (i >= 2)
        pipes_list->nb_pipes = i - 1;
}

void    bloc1_pip_list(t_begin_pipes *pipes_list, t_pipes_part *pipe_part, t_token *token, t_token *ref_token, int i)
{
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
    if (i == 0)
        pipes_list->first = pipe_part;
    pipes_list->nb_mots = 0;
    ref_token = token;
    while (token && ft_strcmp(token->value, "|") != 0)
    {
        pipes_list->nb_mots++;
        token = token->next;
    }
    if (ref_token)
        i = i;
}

void    bloc2_pip_list(t_begin_pipes *pipes_list, t_pipes_part *pipe_part, t_token *ref_token, int i)
{
    t_pipes_part    *current_pipe;
    int             j;

    j = 0;
    while (j < pipes_list->nb_mots)
    {
        pipe_part->words[j] = ref_token->value;
        ref_token = ref_token->next;
        j++;
    }
    pipe_part->words[j] = NULL;
    if (i != 0)
    {
        current_pipe = pipes_list->first;
        while (current_pipe->next)
            current_pipe = current_pipe->next;
        current_pipe->next = pipe_part;
    }
    pipe_part->next = NULL;
}*/