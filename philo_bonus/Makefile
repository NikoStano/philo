# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nistanoj <nistanoj@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/26 22:30:00 by nistanoj          #+#    #+#              #
#    Updated: 2025/10/30 17:04:26 by nistanoj         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 		=	philo_bonus

#  < --- Compilation --- >
CC			=	cc
CFLAGS		=	-Wall -Wextra -Werror -pthread
DEPFLAGS	=	-MMD -MP
COMPILE		=	$(CC) $(CFLAGS) $(DEPFLAGS)
CFLAGS_D	=	-g -fsanitize=thread

INCLUDE		=	include

#  < --- Directories --- >
DIR_OBJS	=	obj/
CORE_DIR	=	src/core/
ROUT_DIR	=	src/routine/
UTILS_DIR	=	src/utils/

#  < --- Sources --- >
# Core
CORE_SRC	=	init.c \
				main.c \
				parse.c \
				process.c \
				time.c
# Routine
ROUT_SRC	=	monitor.c \
				routine_actions.c \
				routine_utils.c \
				routine.c
# Utils
UTILS_SRC	=	utils.c

#  < --- All Sources --- >
SRCS		=	$(addprefix $(CORE_DIR), $(CORE_SRC)) \
				$(addprefix $(ROUT_DIR), $(ROUT_SRC)) \
				$(addprefix $(UTILS_DIR), $(UTILS_SRC))
OBJS 		=	$(SRCS:%.c=$(DIR_OBJS)%.o)
DEPS 		=	$(OBJS:.o=.d)

#  < --- Colors --- >
RED			=	\033[0;31m
GREEN		=	\033[0;32m
L_GREEN		=	\033[1;32m
YELLOW		=	\033[0;33m
BLUE		=	\033[0;34m
MAGENTA		=	\033[0;35m
CYAN		=	\033[0;36m
BOLD		=	\033[1m
RESET		=	\033[0m

#  < --- Rules --- >
all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(CYAN)[ → ] Linking $(BOLD)$(NAME)...$(RESET)"
	@$(COMPILE) $(OBJS) -o $(NAME)
	@echo "$(GREEN)[ ✓ ] $(BOLD)$(NAME)$(GREEN) compiled successfully!$(RESET)"

$(DIR_OBJS)%.o: %.c
	@mkdir -p $(dir $@)
	@echo "$(YELLOW)[ ℹ ] Compiling $(BOLD)$<...$(RESET)"
	@$(COMPILE) -I$(INCLUDE) -c $< -o $@

-include $(DEPS)

clean:
	@rm -rf $(DIR_OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
