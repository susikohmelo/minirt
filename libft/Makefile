# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lfiestas <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/30 07:17:39 by lfiestas          #+#    #+#              #
#    Updated: 2025/01/04 12:56:23 by lfiestas         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
NAME = libft.a
BONUS_SRCS =
SRCS = ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c ft_isascii.c \
	ft_isdigit.c ft_isprint.c ft_itoa.c ft_memchr.c ft_memcmp.c ft_memcpy.c \
	ft_memmove.c ft_memset.c ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
	ft_putstr_fd.c ft_split.c ft_strchr.c ft_strdup.c ft_striteri.c \
	ft_strjoin.c ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strmapi.c \
	ft_strncmp.c ft_strnstr.c ft_strrchr.c ft_strtrim.c ft_substr.c \
	ft_tolower.c ft_toupper.c ft_arena.c ft_isspace.c ft_i_to_str.c \
	ft_strcmp.c ft_fast_strdup.c \
	$(BONUS_SRCS)
OBJS = $(patsubst %.c,%.o,$(SRCS))
CFLAGS = -Wall -Wextra -Werror

MAKEFLAGS += -j6

.PHONY: all clean fclean re bonus

all: CFLAGS += -O3
all: $(NAME)

debug: CFLAGS += -ggdb3 -gdwarf
debug: CFLAGS += -fsanitize=address -fsanitize=undefined -fsanitize=leak
debug: $(NAME)

so:
	$(CC) -c -fPIC $(CFLAGS) $(SRCS)
	$(CC) -nostartfiles -shared -o libft.so $(OBJS)

bonus: BONUS_SRCS = ft_lstadd_back_bonus.c ft_lstadd_front_bonus.c \
	ft_lstclear_bonus.c ft_lstdelone_bonus.c ft_lstiter_bonus.c \
	ft_lstlast_bonus.c ft_lstmap_bonus.c ft_lstnew_bonus.c ft_lstsize_bonus.c
bonus:
	make BONUS_SRCS="$(BONUS_SRCS)"

$(NAME): $(OBJS)
	ar -rcs $(NAME) $^

%.o: %.c libft.h
	$(CC) -c -o $@ $(CFLAGS) $<

clean:
	rm -f *.o

fclean: clean
	rm -f $(NAME) libft.so tests

re: fclean all

.PRECIOUS: tests

tests: CFLAGS += -ggdb3 -fsanitize=address -fsanitize=undefined -fsanitize=leak
tests: .tests.c $(NAME)
	norminette
	make bonus CFLAGS="$(CFLAGS)"
	$(CC) -o $@ $(CFLAGS) $^ -lbsd
	./$@
