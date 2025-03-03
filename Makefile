# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lfiestas <lfiestas@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/10 17:13:50 by lfiestas          #+#    #+#              #
#    Updated: 2025/02/20 11:46:41 by lfiestas         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = miniRT

SRCS = main.c minirt_init.c vec3.c vec3_dot_cross_clamp_length_normalize.c \
	vec3_matrices.c vec3_scalar.c hooks.c surface_color.c get_object_normal.c \
	surface_get_colors.c parser.c parse_attributes.c parse_shapes.c \
	get_next_line.c get_next_line_utils.c \
	cast_rays.c render_frame.c intersect_dist.c texture_uv_functions.c \
	parse_texture.c get_uv_coords.c free_textures.c parse_utils.c \
	get_texture_color.c font.c edit_objects.c move_shape.c get_skybox_color.c \
	edit_objects.c edit_shape_objects.c render_shape_text.c render_text.c \
	slider.c cylinder_intersect_dist.c shape_intersect_dist.c load_skybox.c \
	phong.c cast_ray.c assert.c mouse_hook.c

OBJS = $(patsubst %.c,build/%.o,$(SRCS))
MLX = MLX42/build/libmlx42.a
MSYS_VERSION = $(if $(findstring Msys, $(shell uname -o)),$(word 1, $(subst ., ,$(shell uname -r))),0)
CC = cc
CFLAGS = -Iinclude -Wall -Wextra -Werror
LFLAGS = -lm -lpthread
ifeq ($(MSYS_VERSION), 0)
LFLAGS += -lglfw -ldl
SANITIZERS = -fsanitize=address -fsanitize=leak -fsanitize=undefined
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

release: CFLAGS += -O3 -march=native -flto -fno-math-errno
release: $(NAME)

dev: CFLAGS += -O3 -march=native -flto -fno-math-errno -DMRT_FATAL_EXPECT=0
dev: $(NAME)

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
	@echo "\e[32mBuild succeeded\e[0m"

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

debug: CFLAGS += -ggdb3 -gdwarf -DMRT_FATAL_EXPECT=0
debug: CFLAGS += $(SANITIZERS)
debug: $(NAME)

rebug:
	make clean
	make debug

rev:
	make clean
	make dev
