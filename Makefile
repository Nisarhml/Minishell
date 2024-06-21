# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nihamila <nihamila@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/24 16:33:46 by aguezzi           #+#    #+#              #
#    Updated: 2024/06/21 18:13:55 by nihamila         ###   ########.fr        #
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
PATHSS = src/signaux/
PATHSE = src/error/
PATHEX = src/exec/

BUILD_PATHS = $(PATHO)

src =	src/main.c\
		src/lexer/utils_lexer.c\
		src/lexer/lexer.c\
		src/lexer/token.c\
		src/lexer/token_utils.c\
		src/lexer/handle_quote.c\
		src/lexer/str_to_token.c\
		src/lexer/str_to_token_utils.c\
		src/lexer/trim_input.c\
		src/signaux/signaux.c\
		src/expander/expander.c\
		src/expander/dollars_why.c\
		src/expander/expander_utils.c\
		src/exec/minishell.c src/exec/create_lists.c src/exec/tokens_error.c src/exec/cmds_args.c \
		src/exec/infile_outfile.c src/exec/heredocs.c \
		src/exec/open_check_files.c src/exec/paths.c src/exec/fork_part.c src/exec/builtins.c \
		src/exec/export_create.c src/exec/cmd_cd.c src/exec/cmd_export_1.c src/exec/cmd_export_2.c src/exec/cmd_unset.c\

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

$(PATHO)%.o: $(PATHS)%.c | $(PATHO)
	$(compile_template)

$(PATHO)%.o: $(PATHSL)%.c | $(PATHO)
	$(compile_template)

$(PATHO)%.o: $(PATHSB)%.c | $(PATHO)
	$(compile_template)

$(PATHO)%.o: $(PATHSEX)%.c | $(PATHO)
	$(compile_template)

$(PATHO)%.o: $(PATHSS)%.c | $(PATHO)
	$(compile_template)

$(PATHO)%.o: $(PATHSE)%.c | $(PATHO)
	$(compile_template)

$(PATHO)%.o: $(PATHEX)%.c | $(PATHO)
	$(compile_template)

$(NAME): $(LIBFT) $(OBJS) $(HEADER)
	@$(CC) $(FLAGS) $(OBJS) $(LIBFT) $(READLINE_LIB) -o $(NAME)
	@echo ""
	@echo " ${GRAS}${RED}-> COMPILATION TERMINEE${RESET}${GRAS}${GREEN}[MINISHELL]${RESET}"
	@printf " ${RED}${GRAS}[${GREEN}%-23.${BAR}s${RED}] [%d/%d (%d%%)]${RESET}" "-----------------------" ${FICH_COUNT} ${NBR_TOT_FICHIER} ${NBR_COMPILER}
	@echo "\n  ${GRAS}${GREEN}                                            "
	@echo "   __  __ _____ _   _ _____  _____ _    _ ______ _      _      "
	@echo "  |  \/  |_   _| \ | |_   _|/ ____| |  | |  ____| |    | |     "
	@echo "  | \  / | | | |  \| | | | | (___ | |__| | |__  | |    | |     "
	@echo "  | |\/| | | | |     | | |  \___ \|  __  |  __| | |    | |     "
	@echo "  | |  | |_| |_| |\  |_| |_ ____) | |  | | |____| |____| |____ "
	@echo "  |_|  |_|_____|_| \_|_____|_____/|_|  |_|______|______|______|"
	@echo "                                                               "
	@echo "\n${RESET}                                                     "
#@echo "${UP}${UP}${UP}"

$(LIBFT):
	@$(MAKE) -s -C $(LIBFTP)

$(BUILD_PATHS):
	@$(MKDIR) -p $(BUILD_PATHS)

clean:
	@rm -f $(OBJS)
	@rmdir $(BUILD_PATHS)
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
