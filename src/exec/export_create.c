/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_create.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nihamila <nihamila@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 18:44:38 by aguezzi           #+#    #+#             */
/*   Updated: 2024/06/26 17:07:54 by nihamila         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*g_var_value(char *variable)
{
	char	*value;

	value = ft_strdup(variable);
	if (!value)
		exit(1);
	value[ft_strchr(value, '=') - value] = '\0';
	return (value);
}

t_var_export	*init_var_export(char *env_var)
{
	t_var_export	*var;

	var = malloc(sizeof(*var));
	if (!var)
		exit(1);
	var->variable = ft_strdup(env_var);
	if (!var->variable)
		exit(1);
	var->next = NULL;
	return (var);
}

void	insert_var_in_order(t_var_export *ref, t_var_export *var, char *value)
{
	t_var_export	*save;
	char			*value_ref;

	while (ref->next)
	{
		value_ref = g_var_value(ref->next->variable);
		if (ft_strcmp(value, value_ref) < 0)
		{
			free(value_ref);
			break ;
		}
		ref = ref->next;
		free(value_ref);
	}
	save = ref->next;
	ref->next = var;
	var->next = save;
}

void	insert_var_sorted(t_begin_export *export, t_var_export *var)
{
	t_var_export	*ref;
	char			*value;

	value = g_var_value(var->variable);
	ref = export->first;
	if (ft_strcmp(value, ref->variable) < 0)
	{
		export->first = var;
		var->next = ref;
	}
	else
		insert_var_in_order(ref, var, value);
	free(value);
}

void	process_env_variable(t_begin_export *export, char *env_var, int index)
{
	t_var_export	*var;

	var = init_var_export(env_var);
	if (index == 0)
		export->first = var;
	else
		insert_var_sorted(export, var);
}

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
