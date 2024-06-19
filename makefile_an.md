NAME	= minishell

SRCS	=	src/main.c src/utils/get_env.c src/utils/utils_lexer.c src/lexer/lexer.c \
			src/lexer/token.c src/lexer/token_utils.c src/lexer/handle_quote.c \
			src/lexer/str_to_token.c src/expander/expander.c src/expander/expander_utils.c \
			src/exec/minishell.c src/exec/create_lists.c src/exec/tokens_error.c src/exec/cmds_args.c \
			src/exec/infile_outfile.c src/exec/heredocs.c src/exec/debugg.c \
		  	src/exec/open_check_files.c src/exec/paths.c src/exec/fork_part.c src/exec/builtins.c \
		   	src/exec/export_create.c src/exec/cmd_cd.c src/exec/cmd_export_1.c src/exec/cmd_export_2.c src/exec/cmd_unset.c

CC	= gcc

FLAGS	= -Wall -Wextra -Werror -lreadline

OBJS	= ${SRCS:.c=.o}

RM	= rm -f

all:	${NAME}

${NAME}:	${OBJS}
		@make -C includes/libft
		@${CC} ${FLAGS} ${OBJS} includes/libft/libft.a -o ${NAME}

clean:
		@echo "clean all..."
		@${RM} ${OBJS} ${OBJS_BONUS}
		@make clean -C includes/libft

fclean:	clean
		@echo "fclean all..."
		@${RM} ${NAME} ${NAME_BONUS}
		@${RM} includes/libft/libft.a

re:	fclean all

.PHONY:	all clean fclean re