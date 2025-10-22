# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nistanoj <nistanoj@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/07 14:04:13 by nistanoj          #+#    #+#              #
#    Updated: 2025/10/22 18:07:29 by nistanoj         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 		=	philo

#  < --- Compilation --- >
CC			=	cc
CFLAGS		=	-Wall -Wextra -Werror -pthread
CFLAGS_D	=	-g -fsanitize=thread

INCLUDE		=	include

#  < --- Directories --- >
DIR_OBJS	=	obj/
CORE_DIR	=	src/core/
MONI_DIR	=	src/monitor/
ROUT_DIR	=	src/routine/
UTILS_DIR	=	src/utils/

#  < --- Sources --- >
# Core
CORE_SRC	=	init.c \
				main.c \
				parse.c \
				thread.c \
				time.c
# Monitor
MONI_SRC	=	monitor_check.c \
				monitor.c
# Routine
ROUT_SRC	=	routine_actions.c \
				routine_loop.c \
				routine.c
# Utils
UTILS_SRC	=	utils.c

#  < --- All Sources --- >
SRCS		=	$(addprefix $(CORE_DIR), $(CORE_SRC)) \
				$(addprefix $(MONI_DIR), $(MONI_SRC)) \
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
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)[ ✓ ] $(BOLD)$(NAME) compiled successfully!$(RESET)"

$(DIR_OBJS)%.o: %.c
	@mkdir -p $(dir $@)
	@echo "$(YELLOW)[ ℹ ] Compiling $(BOLD)$<...$(RESET)"
	@$(CC) $(CFLAGS) -MMD -MP -I$(INCLUDE) -c $< -o $@

-include $(DEPS)

clean:
	@echo "$(RED)[🧹 ] Cleaning object files...$(RESET)"
	@rm -rf $(DIR_OBJS)

fclean: clean
	@echo "$(RED)[🧹 ] Cleaning executable...$(RESET)"
	@rm -f $(NAME)
	@rm -rf logs/

re: fclean all

#  <^\____ Norminette check By Me ___/^>
norminette:
	@echo "$(CYAN)[ ℹ ] Running norminette $(BOLD)BY NISTANOJ...$(RESET)"
	@if command -v norminette > /dev/null 2>&1; then \
		NORM_CMD="norminette"; \
	elif command -v python3 -m norminette > /dev/null 2>&1; then \
		NORM_CMD="python3 -m norminette"; \
	else \
		echo "$(RED)[ ✗ ] Norminette is not installed.$(RESET)"; \
		echo "$(CYAN)[ ℹ ] Norminette check uncompleted.$(RESET)"; \
		exit 1; \
	fi; \
	TARGET="$(filter-out $@,$(MAKECMDGOALS))"; \
	if [ -z "$$TARGET" ]; then \
		TARGET="."; \
	else \
		echo "$(YELLOW)[ ℹ ] Checking target(s): $(BOLD)$$TARGET$(RESET)"; \
	fi; \
	OUTPUT=$$($$NORM_CMD $$TARGET 2>&1) ; \
	OUTPUT=$$(echo "$$OUTPUT" | sed '/Setting locale/d'); \
	FILTERED=$$(echo "$$OUTPUT" | grep -v ": OK!"); \
	if [ -n "$${FILTERED}" ]; then \
		echo "$$OUTPUT" | grep -v ": OK!"; \
		echo "$(RED)[ ✗ ] Norminette found errors !$(RESET)"; \
	else \
		echo "$(GREEN)[ ✓ ] Norminette $(BOLD)passed !$(RESET)"; \
	fi;

#  <^\____ Tester By Me ___/^>
test:
	@echo "$(YELLOW)╔════════════════════════════════════╗$(RESET)"
	@echo "$(YELLOW)║      Launching test on philo       ║$(RESET)"
	@echo "$(YELLOW)╚════════════════════════════════════╝$(RESET)"
	@echo "$(CYAN)→ Launching norminette test :$(RESET)"
	@$(MAKE) -s norminette
	@TESTER="./test_philo.sh"; \
	if [ ! -f "$$TESTER" ]; then \
		echo "$(CYAN)[ ℹ ] Cloning philo_tester...$(RESET)"; \
		git clone -q https://github.com/NikoStano/philo_tester.git; \
		cat philo_tester/test_philo.sh > test_philo.sh; \
		chmod +x test_philo.sh; \
		rm -rf philo_tester; \
		echo "$(GREEN)[ ✓ ] philo_tester $(BOLD)cloned successfully!$(RESET)"; \
	fi
	@echo "$(CYAN)[ → ] Recompiling philosophers for tests...$(RESET)"
	@$(MAKE) -s re
	@echo "$(CYAN)[ → ] Running all tests...$(RESET)"
	@./test_philo.sh
	@if [ $$? -ne 0 ]; then \
		echo "$(RED)[ ✗ ] Some tests failed! Cleaning up...$(RESET)"; \
		rm -f test_philo.sh; \
		exit 1; \
	else \
		echo "$(GREEN)[ ✓ ] All tests passed successfully!$(RESET)"; \
	fi
	@echo "$(CYAN)[ ℹ ] Cleaning up...$(RESET)"
	@rm -f test_philo.sh
	@exit 0

.PHONY: all clean fclean re norminette test
