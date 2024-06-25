/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_create.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguezzi <aguezzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 18:44:38 by aguezzi           #+#    #+#             */
/*   Updated: 2024/06/24 19:28:14 by aguezzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void    create_export(t_begin_pipes *pipes_list, char **env)  // je cree ma liste export a partir de l'env et je la trie en meme temps que je place les variables
{
    t_begin_export  *export;
    t_var_export    *var;
    t_var_export    *ref;
    t_var_export    *save;
    char            *value;  // la chaine actuelle s'arretant au '=' que je vais comparer a value_ref
    char            *value_ref;  // la chaine de la liste a l'index de ref s'arretant au '='
    char            *tmp1;
    char            *tmp2;
    int             i;
    
    pipes_list->export_list = malloc(sizeof(t_begin_export));
    export = pipes_list->export_list;
    if (!export)
        exit (1);
    export->first = NULL;
    i = 0;
    while (env[i])
    {
        var = malloc(sizeof(*var));
        if (!var)
            exit (1);
        var->variable = ft_strdup(env[i]);
        if (!var->variable)
            exit (1);
        var->next = NULL;
        if (i == 0)
            export->first = var;
        else
        {
            value = ft_strdup(var->variable);
            if (!value)
                exit (1);
            tmp1 = value;
            value[ft_strchr(value, '=') - value] = '\0';
            ref = export->first;
            if (ft_strcmp(value, ref->variable) < 0)
            {
                export->first = var;
                var->next = ref;
            }
            else
            {
                while (ref->next)
                {
                    value_ref = ft_strdup(ref->next->variable);
                    if (!value_ref)
                        exit (1);
                    tmp2 = value_ref;
                    value_ref[ft_strchr(value_ref, '=') - value_ref] = '\0';
                    if (ft_strcmp(value, value_ref) < 0)
                    {
                        free(tmp2);
                        break;
                    }
                    ref = ref->next;
                    free(tmp2);
                }
                save = ref->next;
                ref->next = var;
                var->next = save;
            }
            free(tmp1);
        }
        i++;
    }
    determine_name_value(pipes_list);
}

void    determine_name_value(t_begin_pipes *pipes_list)
{
    t_var_export    *var;
    
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

void    affich_export_list(t_begin_pipes *pipes_list)
{
    t_var_export    *var;
    
    var = pipes_list->export_list->first;
    while (var)
    {
        if (var->value != NULL)
            printf("declare -x %s=\"%s\"\n", var->name, var->value);
        else
            printf("declare -x %s\n", var->name);
        var = var->next;
    }
	/*printf("\n------\n\n");
	var = pipes_list->export_list->first;
    while (var)
    {
        printf("n = %s / v = %s\n", var->name, var->value);
        var = var->next;
    }*/
}