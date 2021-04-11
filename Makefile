# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fhenrion <fhenrion@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/12/07 11:40:22 by fhenrion          #+#    #+#              #
#    Updated: 2021/04/11 15:19:32 by fhenrion         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ca_crypt
SRC_NAME = ca64 ca_hash64 args progress_bar ca_crypt64
HDR_NAME = ca64 args progress_bar
DEBUG = ca64

SRC = $(addsuffix .c,$(SRC_NAME))
SRC_FOLDER = srcs/
OBJ = $(addsuffix .o,$(SRC_NAME))
OBJ_FOLDER = objs/
HDR = $(addsuffix .h,$(HDR_NAME))
HDR_FOLDER = hdrs/
DOC_FOLDER = doc/

CC = gcc
CFLAGS = -Werror -Wall -Wextra #-fsanitize=address

.PHONY: clean fclean all re bonus

all: $(NAME)
	
debug: $(OBJ_FOLDER)
	$(CC) -c $(CFLAGS) -DDEBUG $(addprefix $(SRC_FOLDER), $(addsuffix .c,$(DEBUG))) -o $(addprefix $(OBJ_FOLDER), $(addsuffix .o,$(DEBUG)))

$(NAME): $(OBJ_FOLDER) $(addprefix $(OBJ_FOLDER), $(OBJ)) $(addprefix $(HDR_FOLDER), $(HDR))
	$(CC) $(CFLAGS) $(addprefix $(OBJ_FOLDER), $(OBJ)) -o $@
	@echo "\033[0;32mCompilation OK\033[0m"

$(OBJ_FOLDER)%.o : $(SRC_FOLDER)%.c $(HDR_FOLDER)%.h
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_FOLDER)%.o : $(SRC_FOLDER)%.c
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_FOLDER):
	mkdir objs

commit:
	git add $(SRC_FOLDER) $(HDR_FOLDER) $(DOC_FOLDER) Makefile README.md
	git commit -m 'makefile_commit'
	git push

clean:
	/bin/rm -rf $(OBJ_FOLDER)
	@echo "\033[0;32mObjects files cleaning OK\033[0m"

fclean: clean
	/bin/rm -f $(NAME)
	@echo "\033[0;32mLibrary cleaning OK\033[0m"

re: fclean $(NAME)