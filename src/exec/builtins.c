/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguezzi <aguezzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 10:13:43 by aguezzi           #+#    #+#             */
/*   Updated: 2024/06/24 14:58:41 by aguezzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int command_env(t_begin_pipes *pipes_list, t_pipes_part *pipe_part)
{
	t_var_env *var;

	if (pipe_part->args[1])
	{
		printf("env: No options or arguments\n");
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
	return (1);
}

int command_pwd(t_begin_pipes *pipes_list, t_pipes_part *pipe_part)
{
	if (pipe_part->args[1])
	{
		if (pipe_part->args[1][0] == '-' && pipe_part->args[1][1]) // check si il y a option avec '-' et au moins 1 caractere derriere
		{
			if (pipe_part->args[1][1] != '-')
			{
				pipe_part->args[1][2] = '\0';
				printf("minishell: pwd: %s: invalid option\n", pipe_part->args[1]);
				return (1);
			}
			else if (pipe_part->args[1][2])
			{
				pipe_part->args[1][2] = '\0';
				printf("minishell: pwd: %s: invalid option\n", pipe_part->args[1]);
				return (1);
			}
		}
	}
	if (pipes_list->pwd)
		printf("%s\n", pipes_list->pwd);
	return (1);
}

int command_echo(t_pipes_part *pipe_part)
{
	int i;
	int saut;

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
		return (1);
	}
	printf("\n");
	return (1);
}

void loop_flag_echo(char **args, int *i)
{
	int j;

	while (1)
	{
		j = 0;
		if (args[*i][0] == '-')
		{
			j++;
			while (args[*i][j] == 'n')
				j++;
			if (args[*i][j])
				return;
		}
		else
			return;
		(*i)++;
	}
}

int	command_exit(t_begin_pipes *pipes_list, t_pipes_part *pipe_part)
{
	int		i;

	if (!pipe_part->args[1]) // ici pas besoin de changer la sortie_error
	{
		printf("exit\n");
		exit (pipes_list->sortie_error % 256);
		return (1);
	}
	else if (!pipe_part->args[2])
	{
		i = 0;
		while (pipe_part->args[1][i])
		{
			if (pipe_part->args[1][i] == '-' || pipe_part->args[1][i] == '+')
				i++;
			if (!(pipe_part->args[1][i] >= '0' && pipe_part->args[1][i] <= '9'))
			{
				pipes_list->sortie_error = 2;
				write(pipe_part->save_stdout, "exit: ", 6);
				write(pipe_part->save_stdout, pipe_part->args[1], ft_strlen(pipe_part->args[1]));
				write(pipe_part->save_stdout, ": numeric argument required\n", 28);
				exit (pipes_list->sortie_error % 256);
				return (1);
			}
			i++;
		}
		printf("exit\n");
		pipes_list->sortie_error = ft_atoi(pipe_part->args[1]) % 256;
		exit (pipes_list->sortie_error % 256);
		return (1);
	}
	else
	{
		i = 0;
		while (pipe_part->args[1][i])
		{
			if (!(pipe_part->args[1][i] >= '0' && pipe_part->args[1][i] <= '9'))
			{
				pipes_list->sortie_error = 2;
				write(pipe_part->save_stdout, "exit: ", 6);
				write(pipe_part->save_stdout, pipe_part->args[1], ft_strlen(pipe_part->args[1]));
				write(pipe_part->save_stdout, ": numeric argument required\n", 28);
				exit (pipes_list->sortie_error % 256);
				return (1);
			}
			i++;
		}
		pipes_list->sortie_error = 1;
		write(pipe_part->save_stdout, "exit: ", 6);
		write(pipe_part->save_stdout, "too many arguments\n", 19);
	}
	return (1);
}

int builtins(t_begin_pipes *pipes_list, t_pipes_part *pipe_part) // mettre les builtins sous forme d'erreur pour le moment.. A GERER plus tard !
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
		return (command_exit(pipes_list, pipe_part));
	return (0);
}
