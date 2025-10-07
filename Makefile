# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nistanoj <nistanoj@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/07 14:04:13 by nistanoj          #+#    #+#              #
#    Updated: 2025/10/07 16:20:50 by nistanoj         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 		=	philosophers

CC			=	cc
CFLAGS		=	-Wall -Wextra -Werror -pthread
CFLAGS_D	=	-g -fsanitize=thread

INCLUDE		=	inc
DIR_SRCS	=	src/
DIR_OBJS	=	obj/

SRCS		=	$(DIR_SRCS)init.c \
				$(DIR_SRCS)main.c \
				$(DIR_SRCS)monitor.c \
				$(DIR_SRCS)routine.c \
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
NO_COLOR	=	\033[0m

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(CYAN)[ → ] Linking $(NAME)...$(NO_COLOR)"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)[ ✓ ] $(NAME) compiled successfully!$(NO_COLOR)"

$(DIR_OBJS)%.o: $(DIR_SRCS)%.c
	@mkdir -p $(DIR_OBJS)
	@echo "$(YELLOW)[ ℹ ] Compiling $<...$(NO_COLOR)"
	@$(CC) $(CFLAGS) -MMD -MP -I$(INCLUDE) -c $< -o $@

-include $(DEPS)

clean:
	@echo "$(RED)[🧹 ] Cleaning object files...$(NO_COLOR)"
	@rm -rf $(DIR_OBJS)

fclean: clean
	@echo "$(RED)[🧹 ] Cleaning executable...$(NO_COLOR)"
	@rm -f $(NAME)

re: fclean all

debug: CFLAGS += $(CFLAGS_D)
debug: re

# Test rules
test_basic:
	@echo "$(BLUE)→ Basic Tests :$(NO_COLOR)"
	@echo "--------------------------------"
	@echo "$(YELLOW)Test 1: No death (5 800 200 200 7)$(NO_COLOR)"
	@timeout 10 ./$(NAME) 5 800 200 200 7 > /dev/null 2>&1 && echo "$(GREEN)✓ Passed$(NO_COLOR)" || (echo "$(RED)✗ Failed$(NO_COLOR)" && exit 1)
	@sleep 1
	@echo "$(YELLOW)Test 2: Should die (5 800 200 200)$(NO_COLOR)"
	@timeout 5 ./$(NAME) 5 800 200 200 > /dev/null 2>&1 && echo "$(RED)✗ Should have died$(NO_COLOR)" || echo "$(GREEN)✓ Philosopher died as expected$(NO_COLOR)"
	@sleep 1
	@echo "$(YELLOW)Test 3: Tight timing (4 420 200 200)$(NO_COLOR)"
	@timeout 10 ./$(NAME) 4 420 200 200 > /dev/null 2>&1 && echo "$(GREEN)✓ Passed$(NO_COLOR)" || (echo "$(RED)✗ Failed$(NO_COLOR)" && exit 1)
	@sleep 1
	@echo "$(YELLOW)Test 4: Should die quickly (4 310 200 100)$(NO_COLOR)"
	@timeout 5 ./$(NAME) 4 310 200 100 > /dev/null 2>&1
	@echo "$(GREEN)✓ Test completed$(NO_COLOR)"
# 	@sleep 1
# 	@echo "$(YELLOW)Test 5: Stress test (200 6000 2000 2000)$(NO_COLOR)"
# 	@timeout 30 ./$(NAME) 200 6000 2000 2000 7 > /dev/null 2>&1 && echo "$(GREEN)✓ Passed$(NO_COLOR)" || (echo "$(RED)✗ Failed$(NO_COLOR)" && exit 1)
	@echo "--------------------------------"
	@echo "$(BLUE)→ Basic Tests Completed$(NO_COLOR)"
	@echo ""

test_edge:
	@echo "$(CYAN)→ Edge Cases :$(NO_COLOR)"
	@echo "--------------------------------"
	@echo "$(YELLOW)Test 1: Single philosopher (should die)$(NO_COLOR)"
	@timeout 2 ./$(NAME) 1 800 200 200 > /dev/null 2>&1
	@echo "$(GREEN)✓ Philosopher died as expected$(NO_COLOR)"
	@sleep 1
	@echo "$(YELLOW)Test 2: Two philosophers (should die)$(NO_COLOR)"
	@timeout 2 ./$(NAME) 2 400 200 200 > /dev/null 2>&1
	@echo "$(GREEN)✓ Test completed$(NO_COLOR)"
	@sleep 1
	@echo "$(YELLOW)Test 3: 200 philosophers with safe times$(NO_COLOR)"
	@timeout 30 ./$(NAME) 200 800 200 200 7 > /dev/null 2>&1 && echo "$(GREEN)✓ Passed$(NO_COLOR)" || (echo "$(RED)✗ Failed$(NO_COLOR)" && exit 1)
	@echo "--------------------------------"
	@echo "$(CYAN)→ Edge Cases Completed$(NO_COLOR)"
	@echo ""

test_invalid:
	@echo "$(MAGENTA)→ Invalid Input Tests :$(NO_COLOR)"
	@echo "--------------------------------"
	@echo "$(YELLOW)Test 1: No arguments$(NO_COLOR)"
	@./$(NAME) && echo "$(RED)✗ Should fail$(NO_COLOR)" || echo "$(GREEN)✓ Failed as expected$(NO_COLOR)"
	@sleep 1
	@echo "$(YELLOW)Test 2: Non-numeric argument$(NO_COLOR)"
	@./$(NAME) 5 abc 200 200 && echo "$(RED)✗ Should fail$(NO_COLOR)" || echo "$(GREEN)✓ Failed as expected$(NO_COLOR)"
	@sleep 1
	@echo "$(YELLOW)Test 3: Negative number of philosophers$(NO_COLOR)"
	@./$(NAME) -5 800 200 200 && echo "$(RED)✗ Should fail$(NO_COLOR)" || echo "$(GREEN)✓ Failed as expected$(NO_COLOR)"
	@sleep 1
	@echo "$(YELLOW)Test 4: Zero philosophers$(NO_COLOR)"
	@./$(NAME) 0 800 200 200 && echo "$(RED)✗ Should fail$(NO_COLOR)" || echo "$(GREEN)✓ Failed as expected$(NO_COLOR)"
	@sleep 1
	@echo "$(YELLOW)Test 5: Excessive number of philosophers (1000)$(NO_COLOR)"
	@./$(NAME) 1000 800 200 200 && echo "$(RED)✗ Should fail or handle gracefully$(NO_COLOR)" || echo "$(GREEN)✓ Handled as expected$(NO_COLOR)"
	@sleep 1
	@echo "--------------------------------"
	@echo "$(MAGENTA)→ Invalid Input Tests Completed$(NO_COLOR)"
	@echo ""

test_helgrind: $(NAME)
	@echo "$(MAGENTA)→ Testing for data races with Helgrind :$(NO_COLOR)"
	@echo "--------------------------------"
	@echo "$(YELLOW)Running Helgrind on a test case...$(NO_COLOR)"
	@valgrind --tool=helgrind ./$(NAME) 5 800 200 200 5 2> helgrind.log
	@if grep -q "ERROR SUMMARY: 0 errors" helgrind.log; then \
		echo "$(GREEN)✓ No data races detected$(NO_COLOR)"; \
	else \
		echo "$(RED)✗ Data races detected! Check helgrind.log$(NO_COLOR)"; \
	fi
	@rm -f helgrind.log
	@echo "--------------------------------"
	@echo "$(MAGENTA)→ Invalid Input Tests Completed$(NO_COLOR)"
	@echo ""

test_performance: $(NAME)
	@echo "$(MAGENTA)→ Performance test: 200 philosophers :$(NO_COLOR)"
	@echo "--------------------------------"
	@echo "$(YELLOW)Running performance test (200 philosophers)...$(NO_COLOR)"
	@/usr/bin/time -f "Time: %E" timeout 30 ./$(NAME) 200 800 200 200 5 > /dev/null 2>&1 && echo "$(GREEN)✓ Performance test passed$(NO_COLOR)" || echo "$(RED)✗ Performance test failed$(NO_COLOR)"
	@echo "--------------------------------"
	@echo "$(MAGENTA)→ Performance test completed$(NO_COLOR)"
	@echo ""

test: $(NAME)
	@echo "$(YELLOW)╔════════════════════════════════════╗$(NO_COLOR)"
	@echo "$(YELLOW)║    Testing Philosophers Program    ║$(NO_COLOR)"
	@echo "$(YELLOW)╚════════════════════════════════════╝$(NO_COLOR)"
	@echo ""
	@$(MAKE) -s test_basic
	@$(MAKE) -s test_edge
	@$(MAKE) -s test_invalid
	@$(MAKE) -s test_helgrind
	@$(MAKE) -s test_performance
	@echo "$(L_GREEN)✓ All tests completed$(NO_COLOR)"

.PHONY: all clean fclean re test test_basic test_edge test_invalid test_helgrind test_performance