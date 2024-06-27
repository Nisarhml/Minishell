/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nihamila <nihamila@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 11:39:46 by nihamila          #+#    #+#             */
/*   Updated: 2024/06/27 12:31:49 by nihamila         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int ac, char **av, char **env)
{
	t_begin			*begin_list;
	t_begin_pipes	*pipes_list;

	if (ac != 1 || av[1])
	{
		printf("This program doesn't accept arguments\n");
		exit(EXIT_FAILURE);
	}
	init_structs(&begin_list, &pipes_list, env);
	input_loop(begin_list, pipes_list, env);
	cleanup_and_exit(begin_list, pipes_list, NULL, *get_exit_status());
	return (0);
}

void	init_structs(t_begin **begin_list, t_begin_pipes **pipes_list, \
		char **env)
{
	*begin_list = malloc(sizeof(**begin_list));
	*pipes_list = malloc(sizeof(**pipes_list));
	if (!*begin_list || !*pipes_list)
		exit(EXIT_FAILURE);
	(*begin_list)->first = NULL;
	build_env(*pipes_list, env);
	create_export(*pipes_list, env);
	init_pipes_list(*pipes_list, env);
	handle_prompt();
}

void	input_loop(t_begin *begin_list, t_begin_pipes *pipes_list, char **env)
{
	t_prompt	*prompt_data;

	while (1)
	{
		prompt_data = prompt_user_for_input();
		if (!prompt_data->input)
			cleanup_and_exit(begin_list, pipes_list, \
					prompt_data, *get_exit_status());
		if (!is_valid_input(prompt_data, prompt_data->input))
		{
			free(prompt_data->input);
			free(prompt_data);
			continue ;
		}
		process_input(begin_list, pipes_list, prompt_data->input, env);
		free(prompt_data->input);
		free(prompt_data);
	}
}

int	is_valid_input(t_prompt *prompt_data, char *input)
{
	return (input[0] != '\0' && sp(input)
		&& !special_chr_prompt(prompt_data, input));
}
