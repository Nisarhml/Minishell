/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguezzi <aguezzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 23:59:28 by aguezzi           #+#    #+#             */
/*   Updated: 2024/06/26 22:07:34 by aguezzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*find_path(t_begin_pipes *pipes_list)
{
	t_var_env	*var;

	var = pipes_list->env_list->first;
	while (var)
	{
		if (ft_strncmp("PATH", var->name, 4) == 0)
			return (var->variable + 5);
		var = var->next;
	}
	ft_error("Error : variable PATH not found");
	return (NULL);
}

int	check_if_executable(char *path_cmd)
{
	if (access(path_cmd, X_OK) == 0)
		return (1);
	printf("%s: Permission denied\n", path_cmd);
	set_exit_status(126);
	free(path_cmd);
	return (0);
}

int	check_if_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

char	*search_the_path(t_begin_pipes *pipes_list, t_pipes_part *pipe_part)
{
	int		i;
	char	*tmp;
	char	*path_cmd;

	i = 0;
	if (check_if_builtin(pipe_part->cmd))
		return (0);
	if (access(pipe_part->cmd, X_OK) == 0)
		return (pipe_part->cmd);
	while (pipes_list->cmd_paths[i])
	{
		tmp = ft_strjoin(pipes_list->cmd_paths[i], "/");
		path_cmd = ft_strjoin(tmp, pipe_part->cmd);
		free(tmp);
		if (access(path_cmd, 0) == 0)
		{
			if (check_if_executable(path_cmd))
				return (path_cmd);
			exit (126);
		}
		free(path_cmd);
		i++;
	}
	specific_error(pipe_part->cmd);
	return (0);
}

void	specific_error(char *cmd)
{
	if (ft_strchr(cmd, '/'))
	{
		printf("%s: ", cmd);
		ft_error("No such file or directory");
	}
	else
	{
		printf("%s: ", cmd);
		ft_error("command not found");
	}
}
