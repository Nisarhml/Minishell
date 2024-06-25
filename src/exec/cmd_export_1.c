/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguezzi <aguezzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 14:16:04 by aguezzi           #+#    #+#             */
/*   Updated: 2024/06/25 20:11:08 by aguezzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int command_export(t_begin_pipes *pipes_list, t_pipes_part *pipe_part) // les variables possibles : commence par lettre ou '_', et contient lettres '_' et chiffres
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
		else // je regarde les args un a un pour voir si ils sont corrects, auquel j'ajoute la variable a mon export_list
		{
			check_args_export(pipes_list, pipe_part->args);
			return (1);
		}
	}
	return (1);
}

void check_args_export(t_begin_pipes *pipes_list, char **args)
{
	int i;
	int j;

	i = 1;
	set_exit_status(0);
	while (args[i])
	{
		j = 0;
		while (args[i][j])
		{
			if (!(ft_isalnum(args[i][j]) || args[i][j] == '_' || args[i][j] == '=')
				|| args[i][0] == '=')
			{
				printf("minishell: export: `%s`: not a valid identifier\n", args[i]);
				set_exit_status(1);
				break;
			}
			j++;
		}
		if (!args[i][j]) // si je n'ai pas d'erreur de caractere
			add_arg_export(pipes_list, args[i]);
		i++;
	}
}

void add_arg_export(t_begin_pipes *pipes_list, char *arg) // (tenter de reussir a mettre un \ avant \ et " si on peut)
{
	t_var_export *ref;
	char *name;	 // contiendra le nom de la variable

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
            break;
		ref = ref->next;
	}
	if (!ref)
		add_in_export(pipes_list, arg, name);
	modify_var_env(pipes_list, arg, name);
	free(name);
}

int same_name(t_var_export *ref, char *name, char *arg)
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