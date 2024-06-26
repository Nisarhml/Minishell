/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_create_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguezzi <aguezzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 19:22:11 by aguezzi           #+#    #+#             */
/*   Updated: 2024/06/26 19:24:58 by aguezzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

char	*g_var_value(char *variable)
{
	char	*value;

	value = ft_strdup(variable);
	if (!value)
		exit(1);
	value[ft_strchr(value, '=') - value] = '\0';
	return (value);
}
