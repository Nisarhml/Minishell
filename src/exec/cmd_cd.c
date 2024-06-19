/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguezzi <aguezzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 14:16:16 by aguezzi           #+#    #+#             */
/*   Updated: 2024/06/18 17:58:05 by aguezzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int command_cd(t_begin_pipes *pipes_list, t_pipes_part *pipe_part)
{
	if (pipe_part->args[1])
	{
		if (pipe_part->args[2])
		{
			printf("mnishell: cd: too many arguments\n");
			return (1);
		}
		if (pipe_part->args[1][0] == '/') // on est en chemin absolu
		{
			if (chdir(pipe_part->args[1]) == -1)
				printf("minishell: cd: %s: No such file or directory\n", pipe_part->args[1]);
			else
				modify_pwd_oldpwd(pipes_list, pipe_part->args[1]);
		}
		else
			check_relatif_path(pipes_list, pipe_part->args[1]);
	}
	return (1);
}

void check_relatif_path(t_begin_pipes *pipes_list, char *path)
{
	char *new_path;
	char *save_to_free;

	new_path = ft_strjoin(pipes_list->pwd, "/");
	save_to_free = new_path;
	new_path = ft_strjoin(new_path, path);
	free(save_to_free);
	if (chdir(new_path) == -1)
		printf("minishell: cd: %s: No such file or directory\n", path);
	else
		modify_pwd_oldpwd(pipes_list, new_path);
}

void modify_pwd_oldpwd(t_begin_pipes *pipes_list, char *path)
{
	free(pipes_list->oldpwd);
	pipes_list->oldpwd = ft_strdup(pipes_list->pwd);
	change_oldpwd_path(pipes_list, pipes_list->pwd);
	free(pipes_list->pwd);
	pipes_list->pwd = ft_strdup(path);
	change_pwd_path(pipes_list, path);
}

void change_oldpwd_path(t_begin_pipes *pipes_list, char *path)
{
	t_var_env *var_env;
	t_var_export *var_export;

	var_env = pipes_list->env_list->first;
	while (var_env)
	{
		if (ft_strcmp(var_env->name, "OLDPWD") == 0)
		{
			free(var_env->variable);
			var_env->variable = ft_strjoin("OLDPWD=", path);
		}
		var_env = var_env->next;
	}
	var_export = pipes_list->export_list->first;
	while (var_export)
	{
		if (ft_strcmp(var_export->name, "OLDPWD") == 0)
		{
			free(var_export->variable);
			var_export->variable = ft_strjoin("OLDPWD=", path);
			free(var_export->value);
			var_export->value = ft_strdup(path);
		}
		var_export = var_export->next;
	}
}

void change_pwd_path(t_begin_pipes *pipes_list, char *path)
{
	t_var_env *var_env;
	t_var_export *var_export;

	var_env = pipes_list->env_list->first;
	while (var_env)
	{
		if (ft_strcmp(var_env->name, "PWD") == 0)
		{
			free(var_env->variable);
			var_env->variable = ft_strjoin("PWD=", path);
		}
		var_env = var_env->next;
	}
	var_export = pipes_list->export_list->first;
	while (var_export)
	{
		if (ft_strcmp(var_export->name, "PWD") == 0)
		{
			free(var_export->variable);
			var_export->variable = ft_strjoin("PWD=", path);
			free(var_export->value);
			var_export->value = ft_strdup(path);
		}
		var_export = var_export->next;
	}
}