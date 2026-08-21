# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hgawhari <hgawhari@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/08/21 13:35:04 by hgawhari          #+#    #+#              #
#    Updated: 2026/08/21 13:35:53 by hgawhari         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= codexion
CC			= cc
CFLAGS		= -Wall -Wextra -Werror -pthread -Iinclude

SRCS_DIR	= src
OBJS_DIR	= obj

GREEN		= \033[1;32m
BLUE		= \033[1;34m
RED			= \033[1;31m
RESET		= \033[0m

SRCS			= \
				main.c \
				parsing/parsing.c \
				init/init_data.c \
				coder/init_coders.c \
				dongle/init_dongles.c \
				monitor/monitor.c \
				dongle/acquire_dongles.c \
				dongle/dongle_order.c \
				dongle/release_dongles.c \
				simulation/simulation.c \
				simulation/simulation_threads.c \
				queue/queue.c \
				utils/logger.c \
				utils/ft_atoi.c \
				utils/get_time.c \
				utils/cleanup.c \
				utils/exit.c \
				coder/coder_routine.c \
				scheduler/edf.c \
				scheduler/fifo.c

OBJS		= $(addprefix $(OBJS_DIR)/,$(SRCS:.c=.o))

all: $(NAME)

$(NAME): $(OBJS)
	@printf "  $(GREEN)Linking$(RESET)  %s\n" $@
	@$(CC) $(OBJS) -o $(NAME) -pthread

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(dir $@)
	@printf "  $(BLUE)Compiled$(RESET)  %s\n" $<
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@printf "  $(RED)RM$(RESET)  %s\n" $(OBJS_DIR)
	@rm -rf $(OBJS_DIR)

fclean: clean
	@printf "  $(RED)RM$(RESET)  %s\n" $(NAME)
	@rm -f $(NAME)

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re
