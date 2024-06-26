/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguezzi <aguezzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 21:03:31 by aguezzi           #+#    #+#             */
/*   Updated: 2024/06/26 21:16:27 by aguezzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_list(t_begin *begin_list)
{
	t_token		*tofree;
	t_token		*save;

	tofree = begin_list->first;
	while (tofree)
	{
		save = tofree;
		tofree = tofree->next;
		free(save);
	}
}

int	free_all(t_begin *begin_list, t_begin_pipes *pipes_list, \
				t_prompt *prompt_data)
{
	free(prompt_data);
	free_env_export(pipes_list);
	free(pipes_list->env_list);
	free(pipes_list->export_list);
	free(pipes_list->pwd);
	free(pipes_list->oldpwd);
	if (pipes_list->val_dollr)
		free(pipes_list->val_dollr);
	free(begin_list);
	free(pipes_list);
	printf("exit\n");
	return (0);
}

void	free_env_export(t_begin_pipes *pipes_list)
{
	t_var_env		*var_env;
	t_var_env		*var_env_tofree;
	t_var_export	*var_export;
	t_var_export	*var_export_tofree;

	var_env = pipes_list->env_list->first;
	while (var_env)
	{
		var_env_tofree = var_env;
		var_env = var_env->next;
		free(var_env_tofree->variable);
		free(var_env_tofree->name);
		free(var_env_tofree->tmp_value);
		free(var_env_tofree);
	}
	var_export = pipes_list->export_list->first;
	while (var_export)
	{
		var_export_tofree = var_export;
		var_export = var_export->next;
		free(var_export_tofree->variable);
		free(var_export_tofree->name);
		free(var_export_tofree->tmp_value);
		free(var_export_tofree);
	}
}

void	free_pipes_list(t_begin_pipes *pipes_list)
{
	t_pipes_part	*pipe_part;
	t_pipes_part	*save_tofree;
	int				i;

	pipe_part = pipes_list->first;
	while (pipe_part)
	{
		save_tofree = pipe_part;
		pipe_part = pipe_part->next;
		free(save_tofree);
	}
	part_free(pipes_list);
	if (pipes_list->cmd_paths)
	{
		i = 0;
		while (pipes_list->cmd_paths[i])
		{
			free(pipes_list->cmd_paths[i]);
			i++;
		}
		free(pipes_list->cmd_paths);
		pipes_list->cmd_paths = NULL;
	}
}

void	part_free(t_begin_pipes *pipes_list)
{
	if (pipes_list->p)
		free(pipes_list->p);
	pipes_list->p = NULL;
	if (pipes_list->pids)
		free(pipes_list->pids);
	pipes_list->pids = NULL;
}
