/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguezzi <aguezzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 11:39:46 by nihamila          #+#    #+#             */
/*   Updated: 2024/06/24 12:44:57 by aguezzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int ac, char **av, char **env)
{
	t_begin			*begin_list;
	t_begin_pipes	*pipes_list;
	t_prompt		*prompt_data;
	t_token			*token_list;
	char			*trimmed_str;

	if (ac != 1 || av[1])
	{
		printf("This program doesn't accept arguments\n");
		exit(EXIT_FAILURE);
	}
	begin_list = malloc(sizeof(*begin_list));
	pipes_list = malloc(sizeof(*pipes_list));
	if (!begin_list || !pipes_list)
		exit (1);
	begin_list->first = NULL;
	token_list = NULL;
	build_env(pipes_list, env);  // build env_list
	create_export(pipes_list, env);  // create export_list
	init_pipes_list(pipes_list, env);
	handle_prompt();
	while (1)
	{
		prompt_data = prompt_user_for_input();
		if (!prompt_data->input)
			return ( 0/*free_all(begin_list, pipes_list, prompt_data*/);
		if (prompt_data->input[0] != '\0')
		{
			trimmed_str = trim_input(prompt_data->input);
			if (unclosed_quotes(trimmed_str))
			{
				free(trimmed_str);
				//free(prompt_data->input); check if it's needed to free
				free(prompt_data);
				continue; // Revenir au prompt
			}
			token_list = tokenize_and_process(trimmed_str, pipes_list);
			begin_list->first = token_list;
			//printf("\nliste de mots : \n\n");
			affich_list(begin_list);
			//printf("\n\n");
			if (error_pipe_redir(begin_list))
			{
				free(trimmed_str);
				free(prompt_data->input);
				free(prompt_data);
				continue; // Revenir au prompt
			}
			parser_exec(begin_list, pipes_list, env);
			reinit_exec(pipes_list);
			//free_lexer(&token_list);
			free(trimmed_str);
			token_list = NULL;
			//free(prompt_data->input); check if it's needed to free
			free(prompt_data);
		}
	}
	return (0);
}

/*int	free_all(t_begin *begin_list, t_begin_pipes *pipes_list, t_prompt *prompt_data)
{
	free(prompt_data);
	//free_env_export(pipes_list);
	free(begin_list);
	free(pipes_list);
	printf("exit\n");
	//return (sig); ??
}*/

int	error_pipe_redir(t_begin *begin_list)
{
	t_token	*current;

	current = begin_list->first;
	if (current->value[0] == '|')
	{
		printf("syntax error near unexpected token `|'\n");
		return (1);
	}
	while (current)
	{
		if (ft_strcmp(current->value, "||") == 0)
		{
			printf("syntax error near unexpected token `|'\n");
			return (1);
		}
		if (current->token == 4)
		{
			if (current->next)
			{
				if (current->next->token == 4)
				{
					printf("syntax error near unexpected token ");
					printf("`%c`\n", current->next->value[0]);
					return (1);
				}
			}
		}
		if (current->token == 0 || current->token == 1
			|| current->token == 2 || current->token == 3)
		{
			if (current->next)
			{
				if (current->next->token != 5)
				{
					printf("syntax error near unexpected token ");
					printf("`%c`\n", current->next->value[0]);
					return (1);
				}
			}
			else
			{
				printf("syntax error near unexpected token `newline`\n");
				return (1);
			}
		}
		current = current->next;
	}
	return (0);
}

void	reinit_exec(t_begin_pipes *pipes_list)
{
	t_pipes_part	*pipe_part;

	pipe_part = pipes_list->first;
	free_args_words(pipe_part);
	pipe_part = pipes_list->first;
	while (pipe_part)
	{
		if (pipe_part->infile)
			free(pipe_part->infile);
		if (pipe_part->outfile)
			free(pipe_part->outfile);
		if (pipe_part->path_cmd)
			free(pipe_part->path_cmd);
		if (pipe_part->heredocs)
			free(pipe_part->heredocs);
		pipe_part = pipe_part->next;
	}
	free_pipes_list(pipes_list);
}

void	free_pipes_list(t_begin_pipes *pipes_list)
{
	t_pipes_part	*pipe_part;
	t_pipes_part	*save_tofree;
	int				i;

	pipe_part = pipes_list->first;
	while (pipe_part)
	{
		save_tofree = pipe_part;
		pipe_part = pipe_part->next;
		free(save_tofree);
	}
	if (pipes_list->p)
		free(pipes_list->p);
	pipes_list->p = NULL;
	if (pipes_list->pids)
		free(pipes_list->pids);
	pipes_list->pids = NULL;
	if (pipes_list->cmd_paths)
	{
		i = 0;
		while (pipes_list->cmd_paths[i])
		{
			free(pipes_list->cmd_paths[i]);
			i++;
		}
		free(pipes_list->cmd_paths);
		pipes_list->cmd_paths = NULL;
	}
}

void	free_args_words(t_pipes_part *pipe_part)
{
	int	i;
	while (pipe_part)
	{
		if (pipe_part->words)
		{
			i = 0;
			while (pipe_part->words[i])
			{
				free(pipe_part->words[i]);
				i++;
			}
			free(pipe_part->words);
		}
		if (pipe_part->args)
			free(pipe_part->args);
		pipe_part = pipe_part->next;
	}
}

t_prompt	*prompt_user_for_input(void)
{
	t_prompt	*prompt_data;

	prompt_data = (t_prompt *)malloc(sizeof(t_prompt));
	if (!prompt_data)
		return (NULL);
	prompt_data->input = readline("MINISHELL <{0_0}> $> ");
	add_history(prompt_data->input);
	return (prompt_data);
}
void print_token_list(t_token *elem)
{
	//int i;
	t_token *current;

	current = elem;
	//i = 1;
	while (current)
	{
		printf("%s\n", current->value);
		printf("Type: %d\n\n", current->token);
		/*printf("Element %d\n", i++);
		printf("========\n");
		printf("%s\n", current->value);
		printf("Type: %d\n", current->token);
		printf("========\n");
		printf("next\n");
		printf("========\n");*/
		current = current->next;
	}
}
