/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguezzi <aguezzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 14:17:27 by aguezzi           #+#    #+#             */
/*   Updated: 2024/06/22 16:00:16 by aguezzi          ###   ########.fr       */
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

void check_variable_delete(t_begin_pipes *pipes_list, char *name)
{
	t_var_env *var_env;
	t_var_export *var_export;

	var_env = pipes_list->env_list->first;
	if (var_env)
		delete_in_env(pipes_list, var_env, name);
	var_export = pipes_list->export_list->first;
	if (var_env)
		delete_in_export(pipes_list, var_export, name);
}

void delete_in_env(t_begin_pipes *pipes_list, t_var_env *var, char *name)
{
	if (ft_strcmp(var->name, name) == 0)
	{
		pipes_list->env_list->first = var->next;
		free(var->variable);
		free(var->name);
		free(var->tmp_value);
		free(var);
	}
	else
	{
		while (var->next)
		{
			if (ft_strcmp(var->next->name, name) == 0)
			{
				pipes_list->env_free = var->next;
				var->next = var->next->next;
				free(pipes_list->env_free->variable);
				free(pipes_list->env_free->name);
				free(pipes_list->env_free->tmp_value);
				free(pipes_list->env_free);
				break;
			}
			var = var->next;
		}
	}
}

void delete_in_export(t_begin_pipes *pipes_list, t_var_export *var, char *name)
{
	if (ft_strcmp(var->name, name) == 0)
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
			if (ft_strcmp(var->next->name, name) == 0)
			{
				pipes_list->export_free = var->next;
				var->next = var->next->next;
				free(pipes_list->export_free->variable);
				free(pipes_list->export_free->name);
				free(pipes_list->export_free->tmp_value);
				free(pipes_list->export_free);
				break;
			}
			var = var->next;
		}
	}
}