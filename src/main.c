/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguezzi <aguezzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 11:39:46 by nihamila          #+#    #+#             */
/*   Updated: 2024/06/25 23:07:11 by aguezzi          ###   ########.fr       */
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
			return (free_all(begin_list, pipes_list, prompt_data));
		if (special_chr_prompt(prompt_data, prompt_data->input))
			continue;
		if (prompt_data->input[0] != '\0' && sp(prompt_data->input))
		{
			trimmed_str = trim_input(prompt_data->input);
			if (unclosed_quotes(trimmed_str))
			{
				free(trimmed_str);
				free(prompt_data);
				continue;
			}
			token_list = tokenize_and_process(trimmed_str, pipes_list);
			begin_list->first = token_list;
			if (error_pipe_redir(begin_list))
			{
				free(trimmed_str);
				free(prompt_data);
				free_lexer(&token_list);
				continue;
			}
			parser_exec(begin_list, pipes_list, env);
			reinit_exec(pipes_list);
			free_list(begin_list);
			free(trimmed_str);
			token_list = NULL;
		}
		free(prompt_data->input);
		free(prompt_data);
	}
	return (0);
}

int	sp(char *input)
{
	int	i;

	i = 0;
	while (input[i] == ' ')
		i++;
	if (!input[i])
		return (0);
	return (1);
}

void	free_list(t_begin *begin_list)
{
	t_token		*tofree;
	t_token		*save;
	
	tofree = begin_list->first;
	while (tofree)
	{
		save = tofree;
		tofree = tofree->next;
		free(save);
	}
}

int	special_chr_prompt(t_prompt *prompt_data, char *input)
{
	if (ft_strcmp(input, ".") == 0 || ft_strcmp(input, "..") == 0)
	{
		free(prompt_data);
		return (1);
	}
	else if (ft_strncmp(input, "/", 1) == 0)
	{
		free(prompt_data);
		return (1);
	}
	return (0);
}

int	free_all(t_begin *begin_list, t_begin_pipes *pipes_list, t_prompt *prompt_data)
{
	free(prompt_data);
	free_env_export(pipes_list);
	free(pipes_list->env_list);
	free(pipes_list->export_list);
	free(pipes_list->pwd);
	free(pipes_list->oldpwd);
	if (pipes_list->val_dollr)
		free(pipes_list->val_dollr);
	free(begin_list);
	free(pipes_list);
	printf("exit\n");
	return (0);
}

void	free_env_export(t_begin_pipes *pipes_list)
{
	t_var_env		*var_env;
	t_var_env		*var_env_tofree;
	t_var_export	*var_export;
	t_var_export	*var_export_tofree;
	
	var_env = pipes_list->env_list->first;
	while (var_env)
	{
		var_env_tofree = var_env;
		var_env = var_env->next;
		free(var_env_tofree->variable);
		free(var_env_tofree->name);
		free(var_env_tofree->tmp_value);
		free(var_env_tofree);
	}
	var_export = pipes_list->export_list->first;
	while (var_export)
	{
		var_export_tofree = var_export;
		var_export = var_export->next;
		free(var_export_tofree->variable);
		free(var_export_tofree->name);
		free(var_export_tofree->tmp_value);
		free(var_export_tofree);
	}
}

void	reinit_exec(t_begin_pipes *pipes_list)
{
	t_pipes_part	*pipe_part;

	if (pipes_list->val_dollr)
	{
		free(pipes_list->val_dollr);
		pipes_list->val_dollr = NULL;
	}
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
	 if (prompt_data->input && *prompt_data->input)
	 	add_history(prompt_data->input);
	return (prompt_data);
}