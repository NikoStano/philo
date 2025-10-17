# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nistanoj <nistanoj@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/07 14:04:13 by nistanoj          #+#    #+#              #
#    Updated: 2025/10/17 13:19:40 by nistanoj         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 		=	philo

CC			=	cc
CFLAGS		=	-Wall -Wextra -Werror -pthread
CFLAGS_D	=	-g -fsanitize=thread

INCLUDE		=	include
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
BOLD		=	\033[1m
RESET		=	\033[0m

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(CYAN)[ → ] Linking $(BOLD)$(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)[ ✓ ] $(BOLD)$(NAME) compiled successfully!$(RESET)"

$(DIR_OBJS)%.o: $(DIR_SRCS)%.c
	@mkdir -p $(DIR_OBJS)
	@echo "$(YELLOW)[ ℹ ] Compiling $(BOLD)$<...$(RESET)"
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
	FILTERED=$$(echo "$$OUTPUT" | grep -v ": OK!"); \
	if [ -n "$$FILTERED" ]; then \
		$$NORM_CMD $$TARGET | grep -v ": OK!"; \
		echo "$(RED)[ ✗ ] Norminette found errors !$(RESET)"; \
	else \
		echo "$(GREEN)[ ✓ ] Norminette $(BOLD)passed !$(RESET)"; \
	fi;

test:
	@$(MAKE) -s test 2>&1 | aha -b > result.html
	@echo "$(YELLOW)╔════════════════════════════════════╗$(RESET)"
	@echo "$(YELLOW)║      Launching test on philo       ║$(RESET)"
	@echo "$(YELLOW)╚════════════════════════════════════╝$(RESET)"
	@echo "$(CYAN)→ Launching norminette test :$(RESET)"
	@$(MAKE) -s norminette
#	@echo "$(CYAN)→ Cloning philo_tester...$(RESET)"
#	@git clone -q https://github.com/NikoStano/philo_tester.git
#	@cat philo_tester/test_philo.sh > test_philo.sh
#	@chmod +x test_philo.sh
#	@rm -rf philo_tester
	@echo "$(GREEN)✓ philo_tester cloned successfully!$(RESET)"
	@echo "$(CYAN)→ Recompiling philosophers for tests...$(RESET)"
	@$(MAKE) -s re
	@echo "$(CYAN)→ Running all tests...$(RESET)"
	@./test_philo.sh || true
	@echo "$(CYAN)✓ All tests ran! Cleaning up...$(RESET)"
	@$(MAKE) fclean
#	@rm -f test_philo.sh
	@echo "$(L_GREEN)✓ All tests completed$(RESET)"
	@exit 0

.PHONY: all clean fclean re norminette test

# ============================================================================ #
#                                   NOTES                                      #
# ============================================================================ #

# Build system notes:
# - Use 'make' or 'make all' for normal compilation
# - Use 'make clean' to remove object files
# - Use 'make fclean' to remove object files and the executable
# - Use 'make re' to recompile everything from scratch
# - Use 'make norminette' to check code style with norminette
# - Use 'make test' to run the test suite (requires internet connection for cloning tester)
# - This Makefile supports dependency tracking with -MMD and -MP flags
# - Colored output is used for better readability of build messages
#
# Test suite notes:
# - The test suite clones a separate repository for testing
# - It recompiles the project before running tests
# - Cleans up after tests by removing the executable and test script
# - Ensure you have internet access when running 'make test'
# - The test script is designed to handle failures gracefully
# - Modify the test_philo.sh script as needed for additional tests
# - The test suite provides feedback on the success or failure of tests
# - Use 'make fclean' after testing to ensure a clean state
# - Ensure you have the necessary permissions to execute scripts
# - The test suite is intended for development and debugging purposes
# - Always review test results to identify potential issues
#
# Troubleshooting:
# - If norminette is not found, ensure it is installed and accessible in your PATH
# - If compilation fails, check for syntax errors or missing files
# - For dependency issues, ensure all source files are correctly referenced
# - If tests fail, review the test_philo.sh script for potential issues
# - Consult the documentation for additional help on using the Makefile
# - Reach out to the community or forums for assistance with specific errors
# - Keep your development environment updated to avoid compatibility issues
#
# Author > NISTANOJ
# --------------------------------------------------------------------------- #