# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/10 17:13:50 by lfiestas          #+#    #+#              #
#    Updated: 2025/02/07 11:07:47 by lfiestas         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = miniRT

SRCS = main.c minirt.c vec3.c vec3_scalar.c hooks.c \
	parser.c parse_attributes.c parse_shapes.c \
	get_next_line.c get_next_line_utils.c \
	cast_rays.c render_frame.c intersect_dist.c texture_uv_functions.c \
	parse_texture.c get_uv_coords.c free_textures.c parse_utils.c

OBJS = $(patsubst %.c,build/%.o,$(SRCS))
MLX = MLX42/build/libmlx42.a
MSYS_VERSION = $(if $(findstring Msys, $(shell uname -o)),$(word 1, $(subst ., ,$(shell uname -r))),0)
CC = cc
CFLAGS = -Iinclude -Wall -Wextra -Werror
LFLAGS = -lm -lpthread
ifeq ($(MSYS_VERSION), 0)
LFLAGS += -lglfw -ldl
else
LFLAGS += -lglfw3
FIX_MLX = cd MLX42 \
	&& sed -i '/"\#version 330 core"/d' ./build/mlx_frag_shader.c \
	&& sed -i '/"\#version 330 core"/d' ./build/mlx_vert_shader.c \
	&& cmake --build build
endif

MAKEFLAGS += -j6

.PHONY: all release debug clean flcean re rebug

all: release

release: CFLAGS += -O3 -march=native -ffast-math -flto
release: $(NAME)

libft/libft.a:
	make -C libft

$(MLX):
	git clone https://github.com/codam-coding-college/MLX42.git
	cd MLX42 && cmake -B build
	cd MLX42 && cmake --build build -j4
	$(FIX_MLX)

$(NAME): $(OBJS) libft/libft.a $(MLX)
	@mkdir -p build
	$(CC) -o $@ $^ $(CFLAGS) $(LFLAGS)

build/%.o: src/%.c src/minirt.h src/vec3.h
	@mkdir -p build
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	make clean -C libft
	rm -rf build

fclean: clean
	make fclean -C libft
	rm -rf build $(NAME) MLX42

re:
	make fclean
	make all

debug: CFLAGS += -ggdb3 -gdwarf
debug: CFLAGS += -fsanitize=address -fsanitize=undefined -fsanitize=leak
debug: $(NAME)

rebug:
	make clean
	make debug
