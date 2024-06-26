/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_create.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguezzi <aguezzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 18:44:38 by aguezzi           #+#    #+#             */
/*   Updated: 2024/06/26 19:25:11 by aguezzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	create_export(t_begin_pipes *pipes_list, char **env)
{
	t_begin_export	*export;
	int				i;

	i = 0;
	pipes_list->export_list = malloc(sizeof(t_begin_export));
	export = pipes_list->export_list;
	if (!export)
		exit(1);
	export->first = NULL;
	while (env[i])
	{
		process_env_variable(export, env[i], i);
		i++;
	}
	determine_name_value(pipes_list);
}

void	determine_name_value(t_begin_pipes *pipes_list)
{
	t_var_export	*var;

	var = pipes_list->export_list->first;
	while (var)
	{
		if (ft_strchr(var->variable, '='))
		{
			var->name = ft_strdup(var->variable);
			var->name[ft_strchr(var->name, '=') - var->name] = '\0';
			var->value = ft_strdup(var->variable);
			var->tmp_value = var->value;
			var->value += (ft_strchr(var->value, '=') - var->value + 1);
		}
		else
		{
			var->name = ft_strdup(var->variable);
			var->value = NULL;
		}
		var = var->next;
	}
}

void	affich_export_list(t_begin_pipes *pipes_list)
{
	t_var_export	*var;

	var = pipes_list->export_list->first;
	while (var)
	{
		if (var->value != NULL)
			printf("declare -x %s=\"%s\"\n", var->name, var->value);
		else
			printf("declare -x %s\n", var->name);
		var = var->next;
	}
	set_exit_status(0);
}
