# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nistanoj <nistanoj@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/07 14:04:13 by nistanoj          #+#    #+#              #
#    Updated: 2025/10/07 21:17:57 by nistanoj         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 		=	philosophers

CC			=	cc
# CFLAGS		=	-Wall -Wextra -Werror -pthread 
CFLAGS		=	-Wall -Wextra -Werror -pthread
# CFLAGS_D	=	-g -fsanitize=thread

INCLUDE		=	inc
DIR_SRCS	=	src/
DIR_OBJS	=	obj/

SRCS		=	$(DIR_SRCS)init.c \
				$(DIR_SRCS)parse.c \
				$(DIR_SRCS)main.c \
				$(DIR_SRCS)thread.c \
				$(DIR_SRCS)monitor.c \
				$(DIR_SRCS)monitor_check.c \
				$(DIR_SRCS)routine.c \
				$(DIR_SRCS)routine_loop.c \
				$(DIR_SRCS)routine_actions.c \
				$(DIR_SRCS)time.c \
				$(DIR_SRCS)utils.c

OBJS 		=	$(SRCS:$(DIR_SRCS)%.c=$(DIR_OBJS)%.o)
DEPS 		=	$(OBJS:.o=.d)

# Colors
RED			=	\033[0;31m
GREEN		=	\033[0;32m
L_GREEN		=	\033[1;32m
YELLOW		=	\033[0;33m
BLUE		=	\033[0;34m
MAGENTA		=	\033[0;35m
CYAN		=	\033[0;36m
RESET	=	\033[0m

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(CYAN)[ → ] Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)[ ✓ ] $(NAME) compiled successfully!$(RESET)"

$(DIR_OBJS)%.o: $(DIR_SRCS)%.c
	@mkdir -p $(DIR_OBJS)
	@echo "$(YELLOW)[ ℹ ] Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) -MMD -MP -I$(INCLUDE) -c $< -o $@

-include $(DEPS)

clean:
	@echo "$(RED)[🧹 ] Cleaning object files...$(RESET)"
	@rm -rf $(DIR_OBJS)

fclean: clean
	@echo "$(RED)[🧹 ] Cleaning executable...$(RESET)"
	@rm -f $(NAME)

re: fclean all

norminette:
	@echo "$(CYAN)[ ℹ ] Running norminette...$(RESET)"
	@if command -v python3 >/dev/null 2>&1; then \
		OUTPUT=$$(python3 -m norminette 2>&1 | grep "Error"); \
		if [ -z "$$OUTPUT" ]; then \
			echo "$(GREEN)[ ✓ ] Norminette passed!$(RESET)"; \
		else \
			python3 -m norminette 2>&1 | grep -v "Norme: OK"; \
			echo "$(RED)[ ✗ ] Norminette found errors!$(RESET)"; \
		fi; \
	else \
		echo "$(RED)[ ✗ ] Norminette is not installed.$(RESET)"; \
	fi
	@echo "$(CYAN)[ ℹ ] Norminette check completed.$(RESET)"

test: $(NAME) norminette
	@echo "$(YELLOW)╔════════════════════════════════════╗$(RESET)"
	@echo "$(YELLOW)║   Launching test on philosophers   ║$(RESET)"
	@echo "$(YELLOW)╚════════════════════════════════════╝$(RESET)"
	@echo "$(CYAN)→ Cloning philo_tester...$(RESET)"
	@git clone -q https://github.com/NikoStano/philo_tester.git
	@cat philo_tester/test_philo.sh > test_philo.sh
	@chmod +x test_philo.sh
	@rm -rf philo_tester
	@echo "$(GREEN)✓ philo_tester cloned successfully!$(RESET)"
	@echo "$(CYAN)→ Recompiling philosophers for tests...$(RESET)"
	@$(MAKE) -s re
	@echo "$(CYAN)→ Running all tests...$(RESET)"
	@./test_philo.sh || true
	@echo "$(CYAN)✓ All tests ran! Cleaning up...$(RESET)"
	@$(MAKE) -s fclean
	@rm -f test_philo.sh
	@echo "$(L_GREEN)✓ All tests completed$(RESET)"

.PHONY: all clean fclean re norminette test
