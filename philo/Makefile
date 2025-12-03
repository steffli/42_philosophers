# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: stliu <stliu@student.42heilbronn.de>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/01 12:56:38 by stliu             #+#    #+#              #
#    Updated: 2025/12/01 12:56:38 by stliu            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
CC = cc
FLAGS = -Wall -Wextra -Werror -pthread

SRC = main.c utils.c init.c time.c sim_utils.c

OBJ = $(SRC:.c=.o)

all:	$(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME)

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY:
	all fclean clean re