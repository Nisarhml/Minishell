/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguezzi <aguezzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 16:14:36 by aguezzi           #+#    #+#             */
/*   Updated: 2024/06/24 13:16:31 by aguezzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void    create_heredocs(t_begin_pipes *pipes_list)
{
    t_pipes_part    *pipe_part;
    int             i;
    int             count; // stocker le nombre de heredocs
    
    count = 0;
    pipe_part = pipes_list->first;
    while (pipe_part)
    {
        i = 0;
        while (pipe_part->words[i])
        {
            if (ft_strcmp(pipe_part->words[i], "<<") == 0)
                count++;
            i++;
        }
        pipe_part->heredocs = malloc(sizeof(int) * (count * 2 + 1));
        i = 0;
        count = 0;
        while (pipe_part->words[i])
        {
            if (ft_strcmp(pipe_part->words[i], "<<") == 0)
            {
                printf("\ncount = %d\n", count);
                heredoc(pipe_part, pipe_part->words[i + 1], count);
                count++;
            }
            i++;
        }
        pipe_part->heredocs[count * 2] = -1;
        pipe_part = pipe_part->next;
    }
}

void	heredoc(t_pipes_part *pipe_part, char *end, int count)
{
	char	*line;

	if (pipe(pipe_part->heredocs + (count * 2)) == -1)
		exit (1);
	while (1)
	{
		write(pipe_part->save_stdout, "> ", 2);
		line = get_next_line(0);
        if (!line)
            break;
		else if ((ft_strncmp(line, end, ft_strlen(end)) == 0
				&& ft_strlen(line) == ft_strlen(end) + 1)
			&& line[0] != '\n')
        {
            free(line);
            break ;
        }
		else
			write(pipe_part->heredocs[count * 2 + 1], line, ft_strlen(line));
        free(line);
	}
}