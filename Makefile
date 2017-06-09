# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: elee <marvin@42.fr>                        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/08/27 21:33:26 by elee              #+#    #+#              #
#    Updated: 2017/06/02 17:59:04 by elee             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = fractol

CC = gcc

CFLAGS = -Wall -Wextra -Werror

LINKS = -framework OpenGL -framework AppKit -framework OpenCL -DAPPLE

MLX = ./minilibx

SRCS = main.c hooks.c draw.c util.c gpu.c hsl.c ft_itoa.c

OBJS = $(SRCS:.c=.o)

INCS = includes/

all: $(NAME)

$(NAME):
	mkdir -p build/
	make -C $(MLX)
	$(CC) $(CFLAGS) -c $(addprefix srcs/, $(SRCS)) -I$(INCS) -I$(MLX)
	mv $(OBJS) build/
	$(CC) $(CFLAGS) $(LINKS) -o $(NAME) $(addprefix build/, $(OBJS)) \
		-I$(INCS) -I$(MLX) $(MLX)/libmlx.a

clean:
	make -C $(MLX) clean
	/bin/rm -rf build

fclean: clean
	make -C $(MLX) fclean
	/bin/rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
