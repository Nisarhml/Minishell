/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aguezzi <aguezzi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 11:35:43 by nihamila          #+#    #+#             */
/*   Updated: 2024/06/22 15:58:48 by aguezzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "../includes/libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <errno.h>
# include <sys/wait.h>

typedef enum {
	LESSER,
	GREATER,
	APPEND,
	HEREDOC,
	PIPE,
	WORD,
} token_type;

typedef struct s_token {
	char			*value;
	token_type		token;
	struct s_token	*next;
	struct s_token	*prev;
} t_token;

typedef struct s_prompt {
	char *input;
} t_prompt;

typedef struct s_expander_data
{
	const char *input;
	size_t i;
	char *result;
} t_expander_data;


typedef	struct	s_env{
	char **env;
	char *path;
} t_env;

/*=======================================PROMPT============================================*/

t_prompt	*prompt_user_for_input(void);

/*=====================================UTILS_LEXER.C=======================================*/

char		*mini_strcpy(char *dest, const char *src);
char 		*ft_strjoin_three_parts(const char *s1, const char *s2, const char *s3);
char		*mini_strcat(char *dest, const char *src);

/*=====================================LEXER.C=============================================*/

t_token		*lexer(char *str);
int			find_token_type(char *str);
t_token		*last_elem(t_token **head);
void		add_word(t_token **lexer, char *str, int token);

/*==================================STR_TO_TOKEN.C=========================================*/

t_token		*split_string_into_tokens(char *str);
int			count_words(char *str);
char		*copy_word_from_str(char *str);
void		fill_word_array(char **words, char *str);

/*=====================================TOKEN.C=============================================*/

int			contain_token(char *str);
char		*get_token(char *str, int type);
char		*get_word(char *str);
t_token		*tokenize_and_process(char *str);
void		free_lexer(t_token **tokens);

/*===================================TOKEN_UTILS.C=======================================*/

int			is_token(char c);
int			is_whitespace(char c);
char		*trim_input(char *input);
char		*ft_strjoin_free(char *s1, char *s2, int to_free);

/*=====================================HANDLE_QUOTE.C======================================*/

int			is_in_quote(char c);
int			unclosed_quotes(char *str);
char		**split_str_by_spaces_and_quotes(char *str);

/*=========================================EXPANDER.C======================================*/

char		*get_env_name(char *str);
char		*replace_env_var(char *str, int i);
char		*replace_env_vars(char *str);
void		remove_char_at(char *str);
char		remove_unnecessary_quotes(char *str);

/*====================================EXPANDER_UTILS.C======================================*/

int			is_valid_envchar(char c);
int			is_valid_simple_envchar(char c);
char		env_in_quote(char c, char in_quote);

/*==================================PARSER_BUILTINS_EXEC====================================*/

typedef struct s_begin  // pointeur sur liste de tokens
{
    t_token *first;
}   t_begin;

typedef struct s_var_env
{
    char                    *variable;
    char                    *name;
    char                    *value;
    char                    *tmp_value;
    struct  s_var_env       *next;
}   t_var_env;


typedef struct s_begin_env
{
    t_var_env   *first;
}   t_begin_env;

typedef struct s_var_export
{
    char                    *variable;  // chaine entiere avec nom de variable '=' valeur
    char                    *name;  // nom de la variable
    char                    *value;  // valeur de la variable
    char                    *tmp_value;
    struct  s_var_export    *next;
}   t_var_export;


typedef struct s_begin_export
{
    t_var_export    *first;
}   t_begin_export;


typedef struct s_pipes_part
{
	char				**words;
    char                *infile; // nom du dernier fichier infile
    char                *outfile; // nom du dernier fichier outfile
    char                *cmd;
    char                *path_cmd;
    char                **args;
    int                 nb_arg;
    int                 *heredocs; // je mets les paires de fd pour lire le fd de lecture et fermer au prealable le fd d ecriture
    int                 if_infile; // si == 1 : permet de savoir si je dois faire ma redirection dans un infile < ou pas (finalement ca sert a rien car je check la valeur du fd si c est -1)
    int                 if_heredoc; // si == 1 : alors je vais chercher dans mon tableau de heredocs pour recuperer le fd correspondant et le mettre en tant que infile
    int                 fd[2]; // va contenir mon fd infile fd[0] et mon fd outfile fd[1]
    int                 save_stdout;
    
    // les fd ont la priorite sur les pipes en terme d'entree et de sortie, si infile_fd est != -1 alors ce sera notre entree, pareil pour outfile_fd
    struct s_pipes_part *next;
}   t_pipes_part;

typedef struct s_begin_pipes
{
    t_pipes_part    *first;  //  --  A FREE + FREE le contenu des pipe_part
    int             nb_pipes;
    int             *p; // stocke les pipes par paire avec p[i*2] pour lecture et p[i*2 + 1] pour ecriture  --  A FREE
    char            *path_env; // stocke la valeur de $PATH
    char            **cmd_paths; // split des differents chemins du PATH  --  A FREE
    pid_t           *pids;  //  --  A FREE
    t_begin_export  *export_list;
    t_begin_env     *env_list;
    char            *pwd;
    char            *oldpwd;
    t_var_env       *env_free;
    t_var_export    *export_free;
    int             sortie_error;
    int             _stdout;
    int             _stdin;
}   t_begin_pipes;

// creation de ma liste de base envoye par le lexer + creation de ma liste de tranches de pipe
void    affich_list(t_begin *begin_list);
void    create_tokens(t_begin *begin_list, char **argv);
void    create_mots(char **argv);
void    init_pipes_list(t_begin_pipes *pipes_list, char **env);
void    affich_pipes_list(t_begin_pipes *pipes_list);
void    create_pipes_list(t_begin *begin_list, t_begin_pipes *pipes_list);

// check error for pipes and redir
int		error_pipe_redir(t_begin *begin);
void    ft_error(char *s);

// check tokens
void    check_infile_part(t_begin_pipes *pipes_list);
void    check_outfile_part(t_begin_pipes *pipes_list);
void    affich_infiles_outfiles(t_begin_pipes *pipes_list);
void    check_cmds_args(t_begin_pipes *pipes_list);
void    create_part_args(t_pipes_part *pipe_part);
void    affich_cmds_args(t_begin_pipes *pipes_list);

// handle heredocs, files and check errors
void    create_heredocs(t_begin_pipes *pipes_list);
void	heredoc(t_pipes_part *pipe_part, char *end, int count);
int     open_infile(t_pipes_part *pipe_part, char *infile);
int     open_outfile(t_pipes_part *pipe_part, char *outfile, char *redir);
//void    read_heredoc(t_begin_pipes *pipes_list);  //debuggage heredocs

// export and env
void    create_export(t_begin_pipes *pipes_list, char **env);
void    determine_name_value(t_begin_pipes *pipes_list);
void	check_args_export(t_begin_pipes *pipes_list, char **args);
void	add_arg_export(t_begin_pipes *pipes_list, char *arg);
int     same_name(t_var_export *ref, char *name, char *arg, char *value);
void	add_in_export(t_begin_pipes *pipes_list, char *arg, char *name, char *value);
int     cond(t_begin_pipes *pipes_list, t_var_export *ref,\
		 t_var_export *new, char *name);
void    affich_export_list(t_begin_pipes *pipes_list);
void    build_env(t_begin_pipes *pipes_list, char **env);
void    affich_env_list(t_begin_pipes *pipes_list);
void	modify_env(t_begin_pipes *pipes_list, char *variable, char *name);
void	add_in_env(t_begin_pipes *pipes_list, char *variable, char *name);

// Paths
char	*find_path(t_begin_pipes *pipes_list);
char	*search_the_path(t_begin_pipes *pipes_list, t_pipes_part *pipe_part);
void	specific_error(char *cmd);

// FORK part
void    begin_forks(t_begin_pipes *pipes_list, char **env);
void    child_process(t_begin_pipes *pipes_list, t_pipes_part *pipe_part, char **env, int i);
int     open_close_files(t_pipes_part *pipe_part);
void    affich_fd_pipes(t_begin_pipes *pipes_list); // debuggage
void    create_pipes(t_begin_pipes *pipes_list);
void    define_infile_outfile(t_begin_pipes *pipes_list, t_pipes_part *pipe_part, int i);
int     last_heredoc(t_pipes_part *pipe_part);
void    close_pipes_child(t_begin_pipes *pipes_list, t_pipes_part *pipe_part);
void    close_pipes_parent(t_begin_pipes *pipes_list);
void	wait_childs(t_begin_pipes *pipes_list);
void    exec_no_pipe(t_begin_pipes *pipes_list, char **env);
int     prepa_builtin_solo(t_begin_pipes *pipes_list, t_pipes_part *pipe_part);

// builtins
int	    builtins(t_begin_pipes *pipes_list, t_pipes_part *pipe_part);
int     command_pwd(t_begin_pipes *pipes_list, t_pipes_part *pipe_part);
int     command_export(t_begin_pipes *pipes_list, t_pipes_part *pipe_part);
int     command_env(t_begin_pipes *pipes_list, t_pipes_part *pipe_part);
int 	command_unset(t_begin_pipes *pipes_list, t_pipes_part *pipe_part);
void    check_variable_delete(t_begin_pipes *pipes_list, char *name);
void	delete_in_env(t_begin_pipes *pipes_list, t_var_env *var, char *name);
void	delete_in_export(t_begin_pipes *pipes_list, t_var_export *var, char *name);
int	    command_echo(t_pipes_part *pipe_part);
void	loop_flag_echo(char **args, int *i);
int	    command_cd(t_begin_pipes *pipes_list, t_pipes_part *pipe_part);
void	modify_pwd_oldpwd(t_begin_pipes *pipes_list, char *path);
void	change_oldpwd_path(t_begin_pipes *pipes_list, char *path);
void	change_pwd_path(t_begin_pipes *pipes_list, char *path);
void	check_relatif_path(t_begin_pipes *pipes_list, char *path);
int     command_exit(t_begin_pipes *pipes_list, t_pipes_part *pipe_part);

// Fonctions pour la partie exec_builtins
void    parser_exec(t_begin *begin_list, t_begin_pipes *pipes_list, char **env);
void	reinit_exec(t_begin_pipes *pipes_list);
void	free_pipes_list(t_begin_pipes *pipes_list);
void	free_args_words(t_pipes_part *pipe_part);

#endif
