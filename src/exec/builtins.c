/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nihamila <nihamila@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 10:13:43 by aguezzi           #+#    #+#             */
/*   Updated: 2024/06/26 11:56:00 by nihamila         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	command_env(t_begin_pipes *pipes_list, t_pipes_part *pipe_part)
{
	t_var_env	*var;

	if (pipe_part->args[1])
	{
		if (pipe_part->args[1][0] == '-')
		{
			printf("env: invalid option\n");
			set_exit_status(125);
			return (1);
		}
		printf("env: `%s` No such file or directory\n", pipe_part->args[1]);
		set_exit_status(127);
		return (1);
	}
	var = pipes_list->env_list->first;
	if (!var)
		printf("\n");
	while (var)
	{
		printf("%s\n", var->variable);
		var = var->next;
	}
	set_exit_status(0);
	return (1);
}

int	command_pwd(t_begin_pipes *pipes_list, t_pipes_part *pipe_part)
{
	if (pipe_part->args[1])
	{
		if (pipe_part->args[1][0] == '-' && pipe_part->args[1][1])
		{
			if (pipe_part->args[1][1] != '-')
			{
				pipe_part->args[1][2] = '\0';
				printf("minishell: pwd:%s: invalid opt\n", pipe_part->args[1]);
				set_exit_status(2);
				return (1);
			}
			else if (pipe_part->args[1][2])
			{
				pipe_part->args[1][2] = '\0';
				printf("minishell: pwd:%s: invalid opt\n", pipe_part->args[1]);
				set_exit_status(2);
				return (1);
			}
		}
	}
	if (pipes_list->pwd)
		printf("%s\n", pipes_list->pwd);
	set_exit_status(0);
	return (1);
}

int	command_echo(t_pipes_part *pipe_part)
{
	int	i;
	int	saut;

	if (pipe_part->args[1])
	{
		i = 1;
		saut = 1;
		loop_flag_echo(pipe_part->args, &i);
		if (i > 1)
			saut = 0;
		while (pipe_part->args[i])
		{
			printf("%s", pipe_part->args[i]);
			if (pipe_part->args[i + 1])
				printf(" ");
			i++;
		}
		if (saut)
			printf("\n");
		set_exit_status(0);
		return (1);
	}
	printf("\n");
	set_exit_status(0);
	return (1);
}

void	loop_flag_echo(char **args, int *i)
{
	int	j;

	while (1)
	{
		j = 0;
		if (args[*i][0] == '-')
		{
			j++;
			while (args[*i][j] == 'n')
				j++;
			if (args[*i][j])
				return ;
		}
		else
			return ;
		(*i)++;
	}
}

int	builtins(t_begin *begin_list, t_begin_pipes *pipes_list, \
			t_pipes_part *pipe_part)
{
	if (ft_strcmp(pipe_part->cmd, "echo") == 0)
		return (command_echo(pipe_part));
	else if (ft_strcmp(pipe_part->cmd, "export") == 0)
		return (command_export(pipes_list, pipe_part));
	else if (ft_strcmp(pipe_part->cmd, "unset") == 0)
		return (command_unset(pipes_list, pipe_part));
	else if (ft_strcmp(pipe_part->cmd, "cd") == 0)
		return (command_cd(pipes_list, pipe_part));
	else if (ft_strcmp(pipe_part->cmd, "pwd") == 0)
		return (command_pwd(pipes_list, pipe_part));
	else if (ft_strcmp(pipe_part->cmd, "env") == 0)
		return (command_env(pipes_list, pipe_part));
	else if (ft_strcmp(pipe_part->cmd, "exit") == 0)
		return (command_exit(begin_list, pipes_list, pipe_part));
	return (0);
}
