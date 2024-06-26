/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nihamila <nihamila@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 21:35:52 by aguezzi           #+#    #+#             */
/*   Updated: 2024/06/26 15:48:28 by nihamila         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	command_exit(t_begin *begin_list, t_begin_pipes *pipes_list, \
				t_pipes_part *pipe_part)
{
	if (!pipe_part->args[1])
	{
		if (pipes_list->nb_pipes == 0)
			printf("exit\n");
		set_exit_status(pipes_list->sortie_error % 256);
		free_exit(begin_list, pipes_list);
		exit(pipes_list->sortie_error % 256);
	}
	else if (!pipe_part->args[2])
		bloc_2_exit(begin_list, pipes_list, pipe_part);
	else
	{
		bloc_3_exit(begin_list, pipes_list, pipe_part);
	}
	return (1);
}

void	bloc_2_exit(t_begin *begin_list, t_begin_pipes *pipes_list, \
					t_pipes_part *p)
{
	int	i;

	i = 0;
	while (p->args[1][i])
	{
		if (p->args[1][i] == '-' || p->args[1][i] == '+')
			i++;
		if (!(p->args[1][i] >= '0' && p->args[1][i] <= '9'))
		{
			pipes_list->sortie_error = 2;
			write(p->save_stdout, "exit: ", 6);
			write(p->save_stdout, p->args[1], ft_strlen(p->args[1]));
			write(p->save_stdout, ": numeric argument required\n", 28);
			set_exit_status(pipes_list->sortie_error % 256);
			free_exit(begin_list, pipes_list);
			exit(pipes_list->sortie_error % 256);
		}
		i++;
	}
	if (pipes_list->nb_pipes == 0)
		printf("exit\n");
	pipes_list->sortie_error = ft_atoi(p->args[1]) % 256;
	set_exit_status(pipes_list->sortie_error % 256);
	free_exit(begin_list, pipes_list);
	exit(pipes_list->sortie_error % 256);
}

void	bloc_3_exit(t_begin *begin_list, t_begin_pipes *pipes_list, \
					t_pipes_part *pipe_part)
{
	int	i;

	i = 0;
	while (pipe_part->args[1][i])
	{
		if (!(pipe_part->args[1][i] >= '0' && pipe_part->args[1][i] <= '9'))
		{
			pipes_list->sortie_error = 2;
			write(pipe_part->save_stdout, "exit: ", 6);
			write(pipe_part->save_stdout, pipe_part->args[1], \
				ft_strlen(pipe_part->args[1]));
			write(pipe_part->save_stdout, ": numeric argument required\n", 28);
			set_exit_status(pipes_list->sortie_error % 256);
			free_exit(begin_list, pipes_list);
			exit(pipes_list->sortie_error % 256);
		}
		i++;
	}
	pipes_list->sortie_error = 1;
	write(pipe_part->save_stdout, "exit: ", 6);
	write(pipe_part->save_stdout, "too many arguments\n", 19);
	set_exit_status(1);
}

void	free_exit(t_begin *begin_list, t_begin_pipes *pipes_list)
{
	reinit_exec(pipes_list);
	free_list(begin_list);
}
