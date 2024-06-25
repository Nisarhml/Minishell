/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguezzi <aguezzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 14:16:16 by aguezzi           #+#    #+#             */
/*   Updated: 2024/06/24 19:09:59 by aguezzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int command_cd(t_begin_pipes *pipes_list, t_pipes_part *pipe_part)
{
	char	pwd[1024];

	if (pipe_part->args[1])
	{
		if (chdir(pipe_part->args[1]) == -1)
			printf("minishell: cd: %s: No such file or directory\n", pipe_part->args[1]);
		else
		{
			if (pipes_list->oldpwd)
				free(pipes_list->oldpwd);
			pipes_list->oldpwd = ft_strdup(pipes_list->pwd);
			if (getcwd(pwd, sizeof(pwd)) != NULL)
			{
				printf("OKOK\n");
				if (pipes_list->pwd)
					free(pipes_list->pwd);
				pipes_list->pwd = ft_strdup(pwd);
				change_pwd_path(pipes_list, pipes_list->pwd);
			}
			change_oldpwd_path(pipes_list, pipes_list->oldpwd);
		}
	}
	return (1);
}

void	change_oldpwd_path(t_begin_pipes *pipes_list, char *path)
{
	t_var_env		*var_env;
	t_var_export	*var_export;

	var_env = pipes_list->env_list->first;
	while (var_env)
	{
		if (ft_strcmp(var_env->name, "OLDPWD") == 0)
		{
			modify_env(var_env, "OLDPWD", path);
			break;
		}
		var_env = var_env->next;
	}
	var_export = pipes_list->export_list->first;
	while (var_export)
	{
		if (ft_strcmp(var_export->name, "OLDPWD") == 0)
		{
			modify_export(var_export, "OLDPWD", path);
			break;
		}
		var_export = var_export->next;
	}
}

void	change_pwd_path(t_begin_pipes *pipes_list, char *path)
{
	t_var_env		*var_env;
	t_var_export	*var_export;

	var_env = pipes_list->env_list->first;
	while (var_env)
	{
		if (ft_strcmp(var_env->name, "PWD") == 0)
		{
			modify_env(var_env, "PWD", path);
			break;
		}
		var_env = var_env->next;
	}
	var_export = pipes_list->export_list->first;
	while (var_export)
	{
		if (ft_strcmp(var_export->name, "PWD") == 0)
		{
			modify_export(var_export, "PWD", path);
			break;
		}
		var_export = var_export->next;
	}
}

void	modify_env(t_var_env *var_env, char *wd, char *path)
{
	if (ft_strcmp(wd, "PWD"))
	{
		free(var_env->variable);
		var_env->variable = ft_strjoin("PWD=", path);
		free(var_env->tmp_value);
		var_env->tmp_value = ft_strjoin("PWD=", path);
		var_env->value = var_env->tmp_value + 4;
	}
	else
	{
		free(var_env->variable);
		var_env->variable = ft_strjoin("OLDPWD=", path);
		free(var_env->tmp_value);
		var_env->tmp_value = ft_strjoin("OLDPWD=", path);
		var_env->value = var_env->tmp_value + 7;
	}
}

void	modify_export(t_var_export *var_export, char *wd, char *path)
{
	if (ft_strcmp(wd, "PWD"))
	{
		free(var_export->variable);
		var_export->variable = ft_strjoin("PWD=", path);
		free(var_export->tmp_value);
		var_export->tmp_value = ft_strjoin("PWD=", path);
		var_export->value = var_export->tmp_value + 4;
	}
	else
	{
		free(var_export->variable);
		var_export->variable = ft_strjoin("OLDPWD=", path);
		free(var_export->tmp_value);
		var_export->tmp_value = ft_strjoin("OLDPWD=", path);
		var_export->value = var_export->tmp_value + 7;
	}
}