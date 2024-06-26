/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nihamila <nihamila@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 22:04:58 by aguezzi           #+#    #+#             */
/*   Updated: 2024/06/26 16:19:05 by nihamila         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	affich_env_list(t_begin_pipes *pipes_list)
{
	t_var_env	*var;

	var = pipes_list->env_list->first;
	while (var)
	{
		printf("%s\n", var->variable);
		var = var->next;
	}
}

void	build_env(t_begin_pipes *pipes_list, char **env)
{
	t_var_env	*var;
	int			i;

	pipes_list->env_list = malloc(sizeof(t_begin_env));
	if (!pipes_list->env_list)
		exit (1);
	pipes_list->env_list->first = NULL;
	i = 0;
	while (env[i])
	{
		var = malloc(sizeof(*var));
		if (!var)
			exit (1);
		b_env(pipes_list, var, env, i);
		i++;
	}
}

void	b_env(t_begin_pipes *p, t_var_env *var, char **e, int i)
{
	t_var_env	*ref;

	var->variable = ft_strdup(e[i]);
	var->name = ft_strdup(var->variable);
	var->name[ft_strchr(var->name, '=') - var->name] = '\0';
	var->value = ft_strdup(var->variable);
	var->tmp_value = var->value;
	var->value += ft_strchr(var->value, '=') - var->value + 1;
	var->next = NULL;
	if (i == 0)
		p->env_list->first = var;
	else
	{
		ref = p->env_list->first;
		while (ref->next)
			ref = ref->next;
		ref->next = var;
	}
}
