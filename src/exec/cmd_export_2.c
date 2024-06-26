/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nihamila <nihamila@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 14:35:47 by aguezzi           #+#    #+#             */
/*   Updated: 2024/06/26 12:19:55 by nihamila         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	modify_var_env(t_begin_pipes *pipes_list, char *variable, char *name)
{
	t_var_env	*var;

	var = pipes_list->env_list->first;
	if (!var)
	{
		if (!(ft_strchr(variable, '=')))
			return ;
		var = malloc(sizeof(t_var_env));
		var->variable = ft_strdup(variable);
		var->tmp_value = ft_strdup(variable);
		var->value = ft_strchr(variable, '=') + 1;
		var->name = ft_strdup(name);
		var->next = NULL;
		return ;
	}
	while (var)
	{
		if (b_modify_var_env(var, variable, name))
			break ;
		var = var->next;
	}
	if (!var && ft_strchr(variable, '='))
		add_in_env(pipes_list, variable, name);
}

int	b_modify_var_env(t_var_env *var, char *variable, char *name)
{
	if (ft_strcmp(name, var->name) == 0)
	{
		if (ft_strchr(variable, '='))
		{
			free(var->variable);
			var->variable = ft_strdup(variable);
			free(var->tmp_value);
			var->tmp_value = ft_strdup(variable);
			var->value = ft_strchr(var->tmp_value, '=') + 1;
		}
		return (1);
	}
	return (0);
}

void	add_in_env(t_begin_pipes *pipes_list, char *variable, char *name)
{
	t_var_env	*var;
	t_var_env	*new;

	new = malloc(sizeof(*new));
	if (!new)
		exit(1);
	new->variable = ft_strdup(variable);
	new->tmp_value = ft_strdup(variable);
	new->value = ft_strchr(new->tmp_value, '=') + 1;
	new->name = ft_strdup(name);
	new->next = NULL;
	var = pipes_list->env_list->first;
	while (var->next)
		var = var->next;
	var->next = new;
}

void	add_in_export(t_begin_pipes *pipes_list, char *arg, char *name)
{
	t_var_export	*ref;
	t_var_export	*new;

	new = malloc(sizeof(*new));
	if (!new)
		exit(1);
	new->variable = ft_strdup(arg);
	new->tmp_value = ft_strdup(arg);
	new->name = ft_strdup(name);
	if (ft_strchr(new->tmp_value, '='))
		new->value = ft_strchr(new->tmp_value, '=') + 1;
	else
		new->value = NULL;
	ref = pipes_list->export_list->first;
	if (cond(pipes_list, ref, new, name))
		return ;
	while (ref->next)
	{
		if (ft_strcmp(new->name, ref->next->name) < 0)
			break ;
		ref = ref->next;
	}
	new->next = ref->next;
	ref->next = new;
}

int	cond(t_begin_pipes *pipes_list, t_var_export *ref, \
		t_var_export *new, char *name)
{
	if (!ref)
	{
		ref = new;
		ref->next = NULL;
		return (1);
	}
	if (ft_strcmp(name, ref->name) < 0)
	{
		pipes_list->export_list->first = new;
		new->next = ref;
		return (1);
	}
	return (0);
}
