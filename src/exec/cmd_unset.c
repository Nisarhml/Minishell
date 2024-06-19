/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguezzi <aguezzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 14:17:27 by aguezzi           #+#    #+#             */
/*   Updated: 2024/06/18 19:27:57 by aguezzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int command_unset(t_begin_pipes *pipes_list, t_pipes_part *pipe_part)
{
	int i;

	if (!pipe_part->args[1])
		printf("\n");
	else
	{
		if (pipe_part->args[1][0] == '-')
		{
			printf("unset: No options\n");
			return (1);
		}
		i = 1;
		while (pipe_part->args[i])
		{
			check_variable_delete(pipes_list, pipe_part->args[i]);
			i++;
		}
	}
	return (1);
}

void check_variable_delete(t_begin_pipes *pipes_list, char *arg)
{
	t_var_env *var_env;
	t_var_export *var_export;

	var_env = pipes_list->env_list->first;
	if (var_env)
		delete_in_env(pipes_list, var_env, arg);
	var_export = pipes_list->export_list->first;
	if (var_env)
		delete_in_export(pipes_list, var_export, arg);
}

void delete_in_env(t_begin_pipes *pipes_list, t_var_env *var, char *arg)
{
	if (ft_strcmp(var->name, arg) == 0)
	{
		pipes_list->env_list->first = var->next;
		free(var->variable);
		free(var->name);
		free(var);
	}
	else
	{
		while (var->next)
		{
			if (ft_strcmp(var->next->name, arg) == 0)
			{
				var->next = var->next->next;
				free(var->variable);
				free(var->name);
				free(var);
				break;
			}
			var = var->next;
		}
	}
}

void delete_in_export(t_begin_pipes *pipes_list, t_var_export *var, char *arg)
{
	if (ft_strcmp(var->name, arg) == 0)
	{
		pipes_list->export_list->first = var->next;
		free(var->variable);
		free(var->name);
		free(var);
	}
	else
	{
		while (var->next)
		{
			if (ft_strcmp(var->next->name, arg) == 0)
			{
				var->next = var->next->next;
				free(var->variable);
				free(var->name);
				free(var);
				break;
			}
			var = var->next;
		}
	}
}