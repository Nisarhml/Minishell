# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aguezzi <aguezzi@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/24 16:33:46 by aguezzi           #+#    #+#              #
#    Updated: 2024/06/18 20:16:25 by aguezzi          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
MKDIR = mkdir

CC = gcc

LIBFTP = includes/libft/
PATHO = build/objs/
PATHS = src/
PATHSL = src/lexer/
PATHSP = src/parser/
PATHSB = src/builtins/
PATHSEX = src/expander/
PATHSU = src/utils/
PATHSE = src/error/
PATHEX = src/exec/

BUILD_PATHS = $(PATHO)

src =	src/main.c\
		src/utils/get_env.c\
		src/utils/utils_lexer.c\
		src/lexer/lexer.c\
		src/lexer/token.c\
		src/lexer/token_utils.c\
		src/lexer/handle_quote.c\
		src/lexer/str_to_token.c\
		src/expander/expander.c\
		src/expander/expander_utils.c\
		src/exec/minishell.c src/exec/create_lists.c src/exec/tokens_error.c src/exec/cmds_args.c \
		src/exec/infile_outfile.c src/exec/heredocs.c \
		src/exec/open_check_files.c src/exec/paths.c src/exec/fork_part.c src/exec/builtins.c \
		src/exec/export_create.c src/exec/cmd_cd.c src/exec/cmd_export_1.c src/exec/cmd_export_2.c src/exec/cmd_unset.c

OBJS = $(addprefix $(PATHO), $(notdir $(patsubst %.c, %.o, $(src))))

FLAGS = -Wall -Werror -Wextra -g #-fsanitize=address

LIBFT = $(LIBFTP)libft.a

HEADER = includes/minishell.h \

#READLINE_DIR = $(shell brew --prefix readline)

READLINE_LIB = -lreadline -lhistory -L $(READLINE_DIR)/lib

INCLUDES = -Iincludes -I$(LIBFTP)includes -I$(READLINE_DIR)/includes

RESET = \033[0m
GRAS = \033[1m
ITALIQUE = \033[3m
SOULIGN = \033[4m
UP = \033[A
BLINK = \033[6m

ORANGE = \033[38;5;216m
DARKBLUE = \033[38;5;21m
RED = \033[38;5;130m
GREEN = \033[38;5;85m

FICH_COUNT = 0
NBR_TOT_FICHIER = 25
NBR_COMPILER = ${shell expr 100 \* ${FICH_COUNT} / ${NBR_TOT_FICHIER}}
BAR =  ${shell expr 25 \* ${FICH_COUNT} / ${NBR_TOT_FICHIER}}

all: $(BUILD_PATHS) $(NAME)

define compile_template
	@$(eval FICH_COUNT = $(shell expr $(FICH_COUNT) + 1))
	@$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@
	@NBR_COMPILER=$$(expr 100 \* $(FICH_COUNT) / $(NBR_TOT_FICHIER))
	@BAR=$$(expr 25 \* $(FICH_COUNT) / $(NBR_TOT_FICHIER))
endef

$(PATHO)%.o: $(PATHS)%.c
	$(compile_template)

$(PATHO)%.o: $(PATHSL)%.c
	$(compile_template)

$(PATHO)%.o: $(PATHSB)%.c
	$(compile_template)

$(PATHO)%.o: $(PATHSEX)%.c
	$(compile_template)

$(PATHO)%.o: $(PATHSU)%.c
	$(compile_template)

$(PATHO)%.o: $(PATHSE)%.c
	$(compile_template)

$(PATHO)%.o: $(PATHEX)%.c
	$(compile_template)

$(NAME): $(LIBFT) $(OBJS) $(HEADER)
	@$(CC) $(FLAGS) $(OBJS) $(LIBFT) $(READLINE_LIB) -o $(NAME)
	@echo ""
	@echo " ${GRAS}${RED}-> COMPILATION TERMINEE${RESET}${GRAS}${GREEN}[MINISHELL]${RESET}"
	@printf " ${RED}${GRAS}[${GREEN}%-23.${BAR}s${RED}] [%d/%d (%d%%)]${RESET}" "-----------------------" ${FICH_COUNT} ${NBR_TOT_FICHIER} ${NBR_COMPILER}
#@echo "${UP}${UP}${UP}"

$(LIBFT):
	@$(MAKE) -s -C $(LIBFTP)

$(PATHB):
	@$(MKDIR) $(PATHB)

$(PATHO):
	@$(MKDIR) $(PATHO)

clean:
	@rm -f $(OBJS)
	@rm -f $(PATHB).tmp*
	@rmdir $(PATHO) $(PATHB)
	@echo "${ORANGE}${GRAS}\t🗑  NETTOYAGE 🗑${RESET}"
	@echo "${YEL}${GRAS} 🗑 Supression des fichiers binaires.🗑${RESET}"
	@make fclean -s -C $(LIBFTP)

fclean: clean
	@rm -f $(NAME)
	@rm -f $(LIBFT)
	@printf $(green)
	@echo "${DARKBLUE}${ITALIQUE} -libft.a est supprimé${RESET}"
	@echo "${DARKBLUE}${ITALIQUE} -${NAME} est supprimé${RESET}"
	@printf $(reset)

re: fclean all

.PRECIOUS: $(PATHO)%.o

black 				=	"[1;30m"
red 				=	"[1;31m"
green 				=	"[1;32m"
yellow 				=	"[1;33m"
blue 				=	"[1;34m"
magenta 			=	"[1;35m"
cyan 				=	"[1;36m"
white 				=	"[1;37m"

reset 				=	"[0m"

GRE		=	\033[32m
BLU		=	\033[34m
YEL		=	\033[33m
EOC		=	\033[0m