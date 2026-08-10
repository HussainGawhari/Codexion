NAME		= codexion
CC			= cc
CFLAGS		= -Wall -Wextra -Werror -pthread -Iinclude -MMD -MP
MAKEFLAGS	= --no-print-directory

SRCS_DIR	= src
OBJS_DIR	= obj

GREEN		= \033[1;32m
BLUE		= \033[1;34m
RED			= \033[1;31m
RESET		= \033[0m

VALGRIND		= valgrind
VALGRIND_FLAGS	= --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1

SRCS			= \
				main.c \
				parsing/parsing.c \
				init/init_data.c \
				init/init_coders.c \
				init/init_dongles.c \
				monitor/coder_routine.c \
				monitor/monitor.c \
				dongle/acquire_dongles.c \
				dongle/dongle_queue.c \
				dongle/release_dongle.c \
				queue/queue.c \
				scheduler/scheduler.c \
				utils/logger.c \
				utils/exit.c \
				utils/ft_atoi.c \
				utils/get_time.c \
				utils/cleanup.c \
				utils/memory.c \
				utils/time.c \
				coder/coder.c \
				coder/coder_lifecycle.c \
				coder/coder_actions.c \
				scheduler/edf.c \
				scheduler/fifo.c

OBJS		= $(addprefix $(OBJS_DIR)/,$(SRCS:.c=.o))
DEPS		= $(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(OBJS)
	@printf "  $(GREEN)LD$(RESET)  %s\n" $@
	@$(CC) $(OBJS) -o $(NAME) -pthread

valgrind: $(NAME)
	@printf "  $(BLUE)VG$(RESET)  %s\n" "./$(NAME)"
	@$(VALGRIND) $(VALGRIND_FLAGS) $(VALGRIND_OPTS) ./$(NAME) $(ARGS)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(dir $@)
	@printf "  $(BLUE)CC$(RESET)  %s\n" $<
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@printf "  $(RED)RM$(RESET)  %s\n" $(OBJS_DIR)
	@rm -rf $(OBJS_DIR)

fclean: clean
	@printf "  $(RED)RM$(RESET)  %s\n" $(NAME)
	@rm -f $(NAME)

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re valgrind
