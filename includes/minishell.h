/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nihamila <nihamila@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 11:35:43 by nihamila          #+#    #+#             */
/*   Updated: 2024/06/27 12:41:19 by nihamila         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "../includes/libft/libft.h"
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <strings.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <dirent.h>
# include <sys/errno.h>
# include <termios.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <errno.h>
# include <sys/wait.h>

typedef enum s_token_type
{
	LESSER,
	GREATER,
	APPEND,
	HEREDOC,
	PIPE,
	WORD,
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	token;
	struct s_token	*next;
}	t_token;

typedef struct s_token_data
{
	char	**word;
	t_token	**lexer;
	int		*j;
	char	*str;
	int		*i;
}	t_token_data;

typedef struct s_prompt
{
	char	*input;
}	t_prompt;

typedef struct s_expander_data
{
	const char	*input;
	size_t		i;
	char		*result;
}	t_expander_data;

typedef struct s_env
{
	char	**env;
	char	*path;
}	t_env;

typedef struct s_begin
{
	t_token	*first;
}	t_begin;

typedef struct s_var_env
{
	char				*variable;
	char				*name;
	char				*value;
	char				*tmp_value;
	struct s_var_env	*next;
}	t_var_env;

typedef struct s_begin_env
{
	t_var_env	*first;
}	t_begin_env;

typedef struct s_var_export
{
	char				*variable;
	char				*name;
	char				*value;
	char				*tmp_value;
	struct s_var_export	*next;
}	t_var_export;

typedef struct s_begin_export
{
	t_var_export	*first;
}	t_begin_export;

typedef struct s_pipes_part
{
	char				**words;
	char				*infile;
	char				*outfile;
	char				*cmd;
	char				*path_cmd;
	char				**args;
	int					nb_arg;
	int					*heredocs;
	int					if_infile;
	int					if_heredoc;
	int					fd[2];
	int					save_stdout;
	int					*type;
	struct s_pipes_part	*next;
}	t_pipes_part;

typedef struct s_begin_pipes
{
	t_pipes_part	*first;
	int				nb_pipes;
	int				*p;
	char			*path_env;
	char			**cmd_paths;
	pid_t			*pids;
	t_begin_export	*export_list;
	t_begin_env		*env_list;
	char			*pwd;
	char			*oldpwd;
	t_var_env		*env_free;
	t_var_export	*export_free;
	int				sortie_error;
	int				nb_mots;
	int				_stdout;
	int				_stdin;
	char			*val_dollr;
	int				i;
	int				ret;
}	t_begin_pipes;

void			init_structs(t_begin **begin_list, \
			t_begin_pipes **pipes_list, char **env);
void			input_loop(t_begin *begin_list, \
			t_begin_pipes *pipes_list, char **env);
void			process_input(t_begin *begin_list, t_begin_pipes *pipes_list, \
			char *input, char **env);
void			cleanup_and_exit(t_begin *begin_list, \
		t_begin_pipes *pipes_list, t_prompt *prompt_data, int exit_code);
int				is_valid_input(t_prompt *prompt_data, char *input);
int				handle_unclosed_quotes(char *input);

/*===============================PROMPT======================================*/

t_prompt		*prompt_user_for_input(void);

/*===========================UTILS_LEXER.C===================================*/

char			*mini_strcpy(char *dest, const char *src);
char			*ft_strjoin_three_part(const char *s1, const char *s2, \
		const char *s3);
char			*mini_strcat(char *dest, const char *src);
char			*mini_strstr(char *str, char *to_find);

/*===============================LEXER.C=====================================*/

t_token			*tokenize_and_process(char *str, t_begin_pipes *pipes_list);
int				find_token_type(char *str);
void			add_word(t_token **lexer, char *str, int token);

/*=============================STR_TO_TOKEN.C================================*/

t_token			*split_string_into_tokens(char *str);
//t_token	*add_token(t_token *lexer, char *word, token_type type);
//void		handle_quote_and_token(char *str, char *word, t_token **lexer);
//t_token	*extract_and_add_token(char *str, char in_quote, t_token *lexer);
//t_token	*process_token(char *str, char *word, t_token *lexer);

/*=============================STR_TO_TOKEN_UTILS.C==========================*/

int				count_words(char *str);
int				skip_spaces(char *str, int i);
void			fill_word_array(char **words, char *str);
char			*copy_word_from_str(char *str);

/*===========================TOKEN.C=========================================*/

int				contain_token(char *str);
char			*get_word(char *str);
void			free_lexer(t_token **tokens);

/*==========================TOKEN_UTILS.C====================================*/

int				is_token(char c);
int				is_whitespace(char c);
char			*ft_strjoin_free(char *s1, char *s2, int to_free);

/*=============================TRIM_INPUT.C==================================*/
char			*trim_input(char *input);
char			manage_quotes(char c, char in_quote);
char			*join_words(char *str, char *new_str);

/*===========================HANDLE_QUOTE.C==================================*/

int				is_in_quote(char c);
int				unclosed_quotes(char *str);
char			**split_str_by_spaces_and_quotes(char *str);

/*===========================EXPANDER.C======================================*/

char			*get_env_name(char *str);
char			*replace_env_var(char *str, int i, t_begin_pipes *pipes_list);
char			*replace_env_vars(char *str, t_begin_pipes *pipes_list);
char			*get_env_value(t_begin_pipes *pipes_list, char *name);
char			remove_unnecessary_quotes(char *str);

/*=====================EXPANDER_UTILS.C======================================*/

void			remove_char_at(char *str);
int				is_valid_envchar(char c);
int				is_valid_simple_envchar(char c);
char			env_in_quote(char c, char in_quote);

/*==============================SIGNAUX.C====================================*/

void			handle_prompt(void);
void			basic_signal(int signal);
void			command_signal(int signal);
void			set_basic_signals(void);
void			set_command_signals(void);
/*==========================SIGNAUX_UTILS.C==================================*/

int				*get_exit_status(void);
void			set_exit_status(int status);
void			here_doc_signal(int signal);
void			set_here_doc_signals(void);
void			ignore_signals(void);

/*=======================PARSER_BUILTINS_EXEC================================*/

int				sp(char *input);
void			init_pipes_list(t_begin_pipes *pipes_list, char **env);
void			create_pipes_list(t_begin *begin_list, \
		t_begin_pipes *pipes_list);
void			add_pipe_part(t_begin_pipes *pipes_list, \
		t_pipes_part *pipe_part, int i);
t_token			*count_and_set_words(t_pipes_part *pipe_part, t_token *token);
t_token			*part_in_while(t_pipes_part *pipe_part, \
		t_token *ref_token, int j);
t_pipes_part	*init_pipe_part(t_begin_pipes *pipes_list, int i);

// check error for pipes and redir
int				error_pipe_redir(t_begin *begin);
int				bloc1_err(t_token *current);
int				bloc2_err(t_token *current);
void			ft_error(char *s);

// check tokens
void			check_infile_part(t_begin_pipes *pipes_list);
void			bloc_redir_in(t_pipes_part *pipe_part, int i);
void			check_outfile_part(t_begin_pipes *pipes_list);
void			check_cmds_args(t_begin_pipes *pipes_list);
int				check_if_redir(t_pipes_part *pipe_part, int *nb_arg, int i);
void			create_part_args(t_pipes_part *pipe_part);
int				mini_bloc_part_args(t_pipes_part *pipe_part, int i, int j);

// handle heredocs, files and check errors
void			create_heredocs(t_begin_pipes *pipes_list);
int				check_n_write_hdoc(t_pipes_part *pipe_part, int count, int i);
void			heredoc(t_pipes_part *pipe_part, char *end, int count);
int				open_infile(t_pipes_part *pipe_part, char *infile);
int				open_outfile(t_pipes_part *pipe_part, char *outfile, \
							char *redir);

// export and env

void			create_export(t_begin_pipes *pipes_list, char **env);
void			process_env_variable(t_begin_export *export, \
									char *env_var, int index);
void			insert_var_sorted(t_begin_export *export, t_var_export *var);
void			insert_var_in_order(t_var_export *ref, t_var_export *var, \
									char *value);
char			*g_var_value(char *variable);
t_var_export	*init_var_export(char *env_var);
void			determine_name_value(t_begin_pipes *pipes_list);
void			check_args_export(t_begin_pipes *pipes_list, char **args);
void			add_arg_export(t_begin_pipes *pipes_list, char *arg);
int				same_name(t_var_export *ref, char *name, char *arg);
void			add_in_export(t_begin_pipes *pipes_list, char *arg, char *name);
int				cond(t_begin_pipes *pipes_list, t_var_export *ref, \
					t_var_export *new, char *name);
void			affich_export_list(t_begin_pipes *pipes_list);
void			build_env(t_begin_pipes *pipes_list, char **env);
void			b_env(t_begin_pipes *p, t_var_env *var, char **e, int i);
void			affich_env_list(t_begin_pipes *pipes_list);
void			modify_var_env(t_begin_pipes *pipes_list, \
							char *variable, char *name);
int				b_modify_var_env(t_var_env *var, char *variable, char *name);
void			add_in_env(t_begin_pipes *pipes_list, char *variable, \
			char *name);

// Paths

char			*find_path(t_begin_pipes *pipes_list);
char			*search_the_path(t_begin_pipes *pipes_list, \
								t_pipes_part *pipe_part);
int				check_if_builtin(char *cmd);
int				check_if_executable(char *path_cmd);
void			specific_error(char *cmd);

// FORK part

void			begin_forks(t_begin *begin_list, \
			t_begin_pipes *pipes_list, char **env);
void			child_process(t_begin *begin_list, t_begin_pipes *pipes_list, \
						t_pipes_part *pipe_part, char **env);
int				open_close_files(t_pipes_part *pipe_part);
void			create_pipes(t_begin_pipes *pipes_list);
void			define_infile_outfile(t_begin_pipes *pipes_list, \
			t_pipes_part *pipe_part, int i);
int				last_heredoc(t_pipes_part *pipe_part);
void			close_pipes_child(t_begin_pipes *pipes_list, \
			t_pipes_part *pipe_part);
void			close_pipes_parent(t_begin_pipes *pipes_list);
void			wait_childs(t_begin_pipes *pipes_list);
void			exec_no_pipe(t_begin *begin_list, \
			t_begin_pipes *pipes_list, char **env);
int				prepa_builtin_solo(t_begin_pipes *pipes_list, \
			t_pipes_part *pipe_part);

// builtins

int				builtins(t_begin *begin_list, t_begin_pipes *pipes_list, \
			t_pipes_part *pipe_part);
int				command_pwd(t_begin_pipes *pipes_list, t_pipes_part *pipe_part);
int				command_export(t_begin_pipes *pipes_list, \
			t_pipes_part *pipe_part);
int				command_env(t_begin_pipes *pipes_list, t_pipes_part *pipe_part);
int				command_unset(t_begin_pipes *pipes_list, \
			t_pipes_part *pipe_part);
void			check_variable_delete(t_begin_pipes *pipes_list, char *name);
void			delete_in_env(t_begin_pipes *pipes_list, t_var_env *var, \
			char *name);
void			delete_in_export(t_begin_pipes *pipes_list, t_var_export *var, \
			char *name);
int				command_echo(t_pipes_part *pipe_part);
void			loop_flag_echo(char **args, int *i);
int				command_cd(t_begin_pipes *pipes_list, t_pipes_part *pipe_part);
void			change_oldpwd_path(t_begin_pipes *pipes_list, char *path);
void			change_pwd_path(t_begin_pipes *pipes_list, char *path);
void			modify_env(t_var_env *var_env, char *wd, char *path);
void			modify_export(t_var_export *var_export, char *wd, char *path);
int				command_exit(t_begin *begin_list, t_begin_pipes *pipes_list, \
			t_pipes_part *pipe_part);
void			bloc_2_exit(t_begin *begin_list, t_begin_pipes *pipes_list, \
			t_pipes_part *pipe_part);
void			bloc_3_exit(t_begin *begin_list, t_begin_pipes *pipes_list, \
			t_pipes_part *pipe_part);
void			free_exit(t_begin *begin_list, t_begin_pipes *pipes_list);

// Fonctions pour la partie exec_builtins

void			parser_exec(t_begin *begin_list, t_begin_pipes *pipes_list, \
			char **env);
void			reinit_exec(t_begin_pipes *pipes_list);
void			free_pipes_list(t_begin_pipes *pipes_list);
void			part_free(t_begin_pipes *pipes_list);
void			free_args_words(t_pipes_part *pipe_part);

// fonctions de free et cas speciaux

int				free_all(t_begin *begin_list, t_begin_pipes *pipes_list, \
			t_prompt *prompt_data);
void			free_list(t_begin *begin_list);
void			free_env_export(t_begin_pipes *pipes_list);
int				special_chr_prompt(t_prompt *prompt_data, char *input);

#endif
