/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nihamila <nihamila@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 14:16:04 by aguezzi           #+#    #+#             */
/*   Updated: 2024/06/26 12:10:43 by nihamila         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	command_export(t_begin_pipes *pipes_list, t_pipes_part *pipe_part)
{
	if (!pipe_part->args[1])
		affich_export_list(pipes_list);
	else
	{
		if (pipe_part->args[1][0] == '-' && pipe_part->args[1][1])
		{
			printf("minishell: export: invalid option\n");
			set_exit_status(2);
			return (1);
		}
		else
		{
			check_args_export(pipes_list, pipe_part->args);
			return (1);
		}
	}
	return (1);
}

void	check_args_export(t_begin_pipes *pipes_list, char **args)
{
	int	i;
	int	j;

	i = 1;
	set_exit_status(0);
	while (args[i])
	{
		j = 0;
		while (args[i][j])
		{
			if (!(ft_isalnum(args[i][j]) || args[i][j] == '_' ||
				args[i][j] == '=')
				|| args[i][0] == '=')
			{
				printf("minishell: export: `%s`: not a valid identifier\n", \
						args[i]);
				set_exit_status(1);
				break ;
			}
			j++;
		}
		if (!args[i][j])
			add_arg_export(pipes_list, args[i]);
		i++;
	}
}

void	add_arg_export(t_begin_pipes *pipes_list, char *arg)
{
	t_var_export	*ref;
	char			*name;

	if (ft_strchr(arg, '='))
	{
		name = ft_strdup(arg);
		name[ft_strchr(name, '=') - name] = '\0';
	}
	else
		name = ft_strdup(arg);
	ref = pipes_list->export_list->first;
	while (ref)
	{
		if (same_name(ref, name, arg))
			break ;
		ref = ref->next;
	}
	if (!ref)
		add_in_export(pipes_list, arg, name);
	modify_var_env(pipes_list, arg, name);
	free(name);
}

int	same_name(t_var_export *ref, char *name, char *arg)
{
	if (ft_strcmp(ref->name, name) == 0)
	{
		if (ft_strchr(arg, '='))
		{
			free(ref->variable);
			free(ref->tmp_value);
			ref->tmp_value = ft_strdup(arg);
			ref->variable = ft_strdup(arg);
			ref->value = ft_strchr(ref->tmp_value, '=') + 1;
		}
		return (1);
	}
	return (0);
}
