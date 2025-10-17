#!/bin/bash

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
MAGENTA='\033[0;35m'
CYAN='\033[0;36m'
BOLD='\033[1m'
NC='\033[0m'

# Test counters
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Program name
PHILO="./philo"

# Check if program exists
if [ ! -f "$PHILO" ]; then
    echo -e "${RED}Error: $PHILO not found. Please compile first with 'make'${NC}"
    exit 1
fi

# Function to check death timing (must be within 10ms of time_to_die)
check_death_timing() {
    local log_file="$1"
    local time_to_die="$2"

    # Extract death line
    local death_line=$(grep "died" "$log_file" | head -n 1)
    if [ -z "$death_line" ]; then
        return 1  # No death detected
    fi

    # Extract timestamp of death
    local death_time=$(echo "$death_line" | awk '{print $1}')

    # Extract philosopher ID
    local philo_id=$(echo "$death_line" | awk '{print $2}')

    # Find last meal time for this philosopher
    local last_meal=$(grep "$philo_id is eating" "$log_file" | tail -n 1 | awk '{print $1}')

    if [ -z "$last_meal" ]; then
        last_meal=0  # Never ate
    fi

    # Calculate time since last meal
    local time_since_meal=$((death_time - last_meal))

    # Check if within acceptable range (time_to_die to time_to_die + 10)
    if [ $time_since_meal -ge $time_to_die ] && [ $time_since_meal -le $((time_to_die + 10)) ]; then
        return 0  # Good timing
    else
        echo -e "${RED}   Death timing: ${time_since_meal}ms (expected: ${time_to_die}ms ± 10ms)${NC}"
        return 1  # Bad timing
    fi
}

# Function to check if all philosophers ate required meals
check_all_ate() {
    local log_file="$1"
    local nb_philos="$2"
    local must_eat="$3"

    local i=1
    while [ $i -le $nb_philos ]; do
        local eat_count=$(grep -c "^[0-9]* $i is eating" "$log_file")
        if [ $eat_count -lt $must_eat ]; then
            echo -e "${RED}   Philosopher $i only ate $eat_count times (expected: $must_eat)${NC}"
            return 1
        fi
        i=$((i + 1))
    done
    return 0
}

# Function to check no message after death
check_no_message_after_death() {
    local log_file="$1"

    # Get line number of death
    local death_line_num=$(grep -n "died" "$log_file" | head -n 1 | cut -d: -f1)

    if [ -z "$death_line_num" ]; then
        return 0  # No death, so OK
    fi

    # Check if there are messages after death
    local total_lines=$(wc -l < "$log_file")
    if [ $total_lines -gt $death_line_num ]; then
        echo -e "${RED}   Found messages after death!${NC}"
        return 1
    fi

    return 0
}

# Enhanced run_test function
run_test() {
    local test_name="$1"
    local expected_result="$2"  # "success" or "death" or "fail"
    shift 2
    local args="$@"
    local args_array=($args)

    TOTAL_TESTS=$((TOTAL_TESTS + 1))

    echo -ne "${YELLOW}Test $TOTAL_TESTS: $test_name${NC}"

    # Run the test with appropriate timeout
    local timeout_duration=20
    if [ "${args_array[0]}" -gt 50 ] && [ ! -z "${args_array[4]}" ]; then
        timeout_duration=30  # More time for many philosophers with meals
    fi

    timeout $timeout_duration $PHILO $args > /tmp/philo_test.log 2>&1
    local exit_code=$?

    # Check result based on expected outcome
    local test_passed=0
    local details=""

    case $expected_result in
        "success")
            # Program should exit normally (0) and no death
            if [ $exit_code -eq 0 ]; then
                # Check for no death
                if ! grep -q "died" /tmp/philo_test.log; then
                    # Check for no errors
                    if ! grep -qi "error" /tmp/philo_test.log; then
                        # If must_eat_count provided, check all ate
                        if [ ! -z "${args_array[4]}" ]; then
                            if check_all_ate /tmp/philo_test.log "${args_array[0]}" "${args_array[4]}"; then
                                test_passed=1
                            fi
                        else
                            test_passed=1
                        fi
                    fi
                else
                    details="Death detected when none expected"
                fi
            else
                details="Exit code: $exit_code"
            fi
            ;;
        "death")
            # Program should detect death with proper timing
            if grep -q "died" /tmp/philo_test.log; then
                if check_death_timing /tmp/philo_test.log "${args_array[1]}"; then
                    if check_no_message_after_death /tmp/philo_test.log; then
                        test_passed=1
                    else
                        details="Messages found after death"
                    fi
                else
                    details="Death timing incorrect"
                fi
            else
                details="No death detected"
            fi
            ;;
        "fail")
            # Program should fail (non-zero exit and error message)
            if [ $exit_code -ne 0 ] && [ $exit_code -ne 124 ]; then
                test_passed=1
            else
                details="Should have failed but didn't"
            fi
            ;;
    esac

    if [ $test_passed -eq 1 ]; then
        PASSED_TESTS=$((PASSED_TESTS + 1))
        echo -e " ${GREEN}✓ PASSED${NC}"
    else
        FAILED_TESTS=$((FAILED_TESTS + 1))
        echo -e " ${RED}✗ FAILED${NC}"
        echo -e "${RED}   Command: $PHILO $args${NC}"
        if [ ! -z "$details" ]; then
            echo -e "${RED}   $details${NC}"
        fi
    fi

    sleep 0.3
}

# Print header
echo -e "${BOLD}${CYAN}"
echo "╔════════════════════════════════════════════════════════╗"
echo "║                                                        ║"
echo "║          PHILOSOPHERS COMPREHENSIVE TESTS              ║"
echo "║                                                        ║"
echo "╚════════════════════════════════════════════════════════╝"
echo -e "${NC}"

# ============================================================================
# INVALID INPUT TESTS
# ============================================================================
echo -e "${MAGENTA}${BOLD}[1] INVALID INPUT TESTS${NC}"
echo "-----------------------------------------------------------"

run_test "Non-numeric argument" "fail" 5 abc 200 200
run_test "Negative philosophers" "fail" -5 800 200 200
run_test "Zero philosophers" "fail" 0 800 200 200
run_test "Negative time_to_die" "fail" 5 -800 200 200
run_test "Zero time_to_die" "fail" 5 0 200 200
run_test "Negative time_to_eat" "fail" 5 800 -200 200
run_test "Negative time_to_sleep" "fail" 5 800 200 -200
run_test "Too many arguments" "fail" 5 800 200 200 7 extra

echo ""

# ============================================================================
# DEATH DETECTION TESTS
# ============================================================================
echo -e "${RED}${BOLD}[2] DEATH DETECTION TESTS${NC}"
echo "-----------------------------------------------------------"

run_test "Single philosopher dies" "death" 1 800 200 200
run_test "Two philos die quickly" "death" 2 200 100 50
run_test "Death with 4 philos" "death" 4 310 200 100
run_test "Very quick death" "death" 3 150 100 50
run_test "Instant death" "death" 5 100 50 25
run_test "Death timing 10ms tolerance" "death" 4 400 200 100

echo ""

# ============================================================================
# NO DEATH TESTS (with meal limit)
# ============================================================================
echo -e "${GREEN}${BOLD}[3] NO DEATH TESTS (with meal limit)${NC}"
echo "-----------------------------------------------------------"

run_test "Basic no death" "success" 5 800 200 200 7
run_test "Tight but safe timing" "success" 4 410 200 200 5
run_test "Comfortable timing" "success" 3 800 200 200 10
run_test "Many meals" "success" 4 800 200 200 50
run_test "Your test case" "success" 5 500 200 200 5
run_test "Minimum viable" "success" 4 420 200 200 3

echo ""

# ============================================================================
# NO DEATH TESTS (without meal limit - should run for 10+ seconds)
# ============================================================================
echo -e "${GREEN}${BOLD}[4] NO DEATH INFINITE TESTS${NC}"
echo "-----------------------------------------------------------"

run_test "Infinite simulation" "success" 5 800 200 200
run_test "Tight infinite" "success" 4 410 200 200

echo ""

# ============================================================================
# STRESS TESTS
# ============================================================================
echo -e "${BLUE}${BOLD}[5] STRESS TESTS${NC}"
echo "-----------------------------------------------------------"

run_test "Many philos = 100" "success" 100 800 200 200 7
run_test "Maximum philos = 200" "success" 200 800 200 200 7
run_test "Long simulation" "success" 10 800 200 200 100
run_test "High concurrency" "success" 50 500 200 200 10

echo ""

# ============================================================================
# TIMING PRECISION TESTS
# ============================================================================
echo -e "${CYAN}${BOLD}[6] TIMING PRECISION TESTS${NC}"
echo "-----------------------------------------------------------"

run_test "Minimum safe timing" "success" 4 410 200 200 5
run_test "Fast cycles" "success" 5 310 100 100 7
run_test "Fast eating" "success" 10 600 100 100 7
run_test "Fast sleeping" "success" 10 600 200 50 7
run_test "Ultra fast" "success" 5 210 50 50 10

echo ""

# ============================================================================
# SUMMARY
# ============================================================================
echo -e "${BOLD}${CYAN}"
echo "╔════════════════════════════════════════════════════════╗"
echo "║                    TEST SUMMARY                        ║"
echo "╚════════════════════════════════════════════════════════╝"
echo -e "${NC}"

echo -e "Total tests:  ${BOLD}$TOTAL_TESTS${NC}"
echo -e "Passed:       ${GREEN}${BOLD}$PASSED_TESTS${NC}"
echo -e "Failed:       ${RED}${BOLD}$FAILED_TESTS${NC}"
echo ""

# Calculate percentage
if [ $TOTAL_TESTS -gt 0 ]; then
    PERCENTAGE=$((PASSED_TESTS * 100 / TOTAL_TESTS))
    echo -e "Success rate: ${BOLD}$PERCENTAGE%${NC}"
    echo ""
fi

# Final result
if [ $FAILED_TESTS -eq 0 ]; then
    echo -e "${GREEN}${BOLD}╔════════════════════════════════════════════════════════╗${NC}"
    echo -e "${GREEN}${BOLD}║                                                        ║${NC}"
    echo -e "${GREEN}${BOLD}║                  ✓ ALL TESTS PASSED!                   ║${NC}"
    echo -e "${GREEN}${BOLD}║                                                        ║${NC}"
    echo -e "${GREEN}${BOLD}╚════════════════════════════════════════════════════════╝${NC}"
    rm -f /tmp/philo_test.log
    exit 0
else
    echo -e "${RED}${BOLD}╔════════════════════════════════════════════════════════╗${NC}"
    echo -e "${RED}${BOLD}║                                                        ║${NC}"
    echo -e "${RED}${BOLD}║                  ✗ TESTS FAILED!                       ║${NC}"
    echo -e "${RED}${BOLD}║                                                        ║${NC}"
    echo -e "${RED}${BOLD}╚════════════════════════════════════════════════════════╝${NC}"
    echo -e "${YELLOW}Check /tmp/philo_test.log for last test details.${NC}"
    cat /tmp/philo_test.log > philo.log
    rm -f /tmp/philo_test.log
    exit 1
fi

# # Colors
# RED='\033[0;31m'
# GREEN='\033[0;32m'
# YELLOW='\033[1;33m'
# BLUE='\033[0;34m'
# MAGENTA='\033[0;35m'
# CYAN='\033[0;36m'
# BOLD='\033[1m'
# NC='\033[0m'

# # Test counters
# TOTAL_TESTS=0
# PASSED_TESTS=0
# FAILED_TESTS=0

# # Program name
# PHILO="./philo"

# # Check if program exists
# if [ ! -f "$PHILO" ]; then
#     echo -e "${RED}Error: $PHILO not found. Please compile first with 'make'${NC}"
#     exit 1
# fi

# # Function to run a test
# run_test() {
#     local test_name="$1"
#     local expected_result="$2"  # "success" or "death" or "fail"
#     shift 2
#     local args="$@"

#     TOTAL_TESTS=$((TOTAL_TESTS + 1))

#     echo -ne "${YELLOW}Test $TOTAL_TESTS: $test_name${NC}"

#     # Run the test with timeout
#     timeout 15 $PHILO $args > /tmp/philo_test.log 2>&1
#     local exit_code=$?

#     # Check result based on expected outcome
#     local test_passed=0

#     case $expected_result in
#         "success")
#             # Program should exit normally (0) or timeout means it's still running (good)
#             if [ $exit_code -eq 0 ] || [ $exit_code -eq 124 ]; then
#                 # Check for no errors in output
#                 if ! grep -qi "error" /tmp/philo_test.log; then
#                     test_passed=1
#                 fi
#             fi
#             ;;
#         "death")
#             # Program should detect death (can exit with 0 or be killed by timeout)
#             if grep -q "died" /tmp/philo_test.log; then
#                 test_passed=1
#             fi
#             ;;
#         "fail")
#             # Program should fail (non-zero exit and error message)
#             if [ $exit_code -ne 0 ] && [ $exit_code -ne 124 ]; then
#                 test_passed=1
#             fi
#             ;;
#     esac

#     if [ $test_passed -eq 1 ]; then
#         PASSED_TESTS=$((PASSED_TESTS + 1))
#         echo -e " ${GREEN}✓ PASSED${NC}"
#     else
#         FAILED_TESTS=$((FAILED_TESTS + 1))
#         echo -e " ${RED}✗ FAILED${NC}"
#         echo -e "${RED}   Command: $PHILO $args${NC}"
#         echo -e "${RED}   Exit code: $exit_code${NC}"
#     fi

#     sleep 0.5
# }

# # Print header
# echo -e "${BOLD}${CYAN}"
# echo "╔════════════════════════════════════════════════════════╗"
# echo "║                                                        ║"
# echo "║          PHILOSOPHERS COMPREHENSIVE TESTS              ║"
# echo "║                                                        ║"
# echo "╚════════════════════════════════════════════════════════╝"
# echo -e "${NC}"

# # ============================================================================
# # INVALID INPUT TESTS
# # ============================================================================
# echo -e "${MAGENTA}${BOLD}[1] INVALID INPUT TESTS${NC}"
# echo "-----------------------------------------------------------"

# run_test "No arguments" "fail"
# run_test "Non-numeric argument" "fail" 5 abc 200 200
# run_test "Negative philosophers" "fail" -5 800 200 200
# run_test "Zero philosophers" "fail" 0 800 200 200
# run_test "Negative time_to_die" "fail" 5 -800 200 200
# run_test "Zero time_to_die" "fail" 5 0 200 200
# run_test "Negative time_to_eat" "fail" 5 800 -200 200
# run_test "Negative time_to_sleep" "fail" 5 800 200 -200
# run_test "Too many arguments" "fail" 5 800 200 200 7 extra

# echo ""

# # ============================================================================
# # DEATH DETECTION TESTS
# # ============================================================================
# echo -e "${RED}${BOLD}[2] DEATH DETECTION TESTS${NC}"
# echo "-----------------------------------------------------------"

# run_test "Single philosopher (must die)" "death" 1 800 200 200
# run_test "Two philosophers (should not die)" "success" 2 400 200 200
# run_test "Should die (5 800 200 200)" "success" 5 800 200 200
# run_test "Quick death (4 310 200 100)" "death" 4 310 200 100
# run_test "Very quick death (3 200 100 100)" "death" 3 200 100 100
# run_test "Instant death (4 100 50 50)" "death" 4 100 50 50

# echo ""

# # ============================================================================
# # NO DEATH TESTS (with meal limit)
# # ============================================================================
# echo -e "${GREEN}${BOLD}[3] NO DEATH TESTS (with meal limit)${NC}"
# echo "-----------------------------------------------------------"

# run_test "No death (5 800 200 200 7)" "success" 5 800 200 200 7
# run_test "Tight timing (4 410 200 200 5)" "success" 4 410 200 200 5
# run_test "Safe timing (3 800 200 200 10)" "success" 3 800 200 200 10
# run_test "Many meals (4 800 200 200 50)" "success" 4 800 200 200 50

# echo ""

# # ============================================================================
# # STRESS TESTS
# # ============================================================================
# echo -e "${BLUE}${BOLD}[4] STRESS TESTS${NC}"
# echo "-----------------------------------------------------------"

# run_test "Many philosophers (100 800 200 200 7)" "success" 100 800 200 200 7
# run_test "Many philosophers (200 800 200 200 7)" "success" 200 800 200 200 7
# run_test "Long simulation (10 800 200 200 100)" "success" 10 800 200 200 100
# run_test "High concurrency (50 410 200 200 10)" "success" 50 410 200 200 10

# echo ""

# # ============================================================================
# # TIMING TESTS
# # ============================================================================
# echo -e "${CYAN}${BOLD}[5] TIMING PRECISION TESTS${NC}"
# echo "-----------------------------------------------------------"

# run_test "Minimum safe time (4 410 200 200 5)" "success" 4 410 200 200 5
# run_test "Just enough time (5 400 100 100 7)" "success" 5 400 100 100 7
# run_test "Fast eating (10 600 100 100 7)" "success" 10 600 100 100 7
# run_test "Fast sleeping (10 600 200 50 7)" "success" 10 600 200 50 7

# echo ""

# # ============================================================================
# # SUMMARY
# # ============================================================================
# echo -e "${BOLD}${CYAN}"
# echo "╔════════════════════════════════════════════════════════╗"
# echo "║                    TEST SUMMARY                        ║"
# echo "╚════════════════════════════════════════════════════════╝"
# echo -e "${NC}"

# echo -e "Total tests:  ${BOLD}$TOTAL_TESTS${NC}"
# echo -e "Passed:       ${GREEN}${BOLD}$PASSED_TESTS${NC}"
# echo -e "Failed:       ${RED}${BOLD}$FAILED_TESTS${NC}"
# echo ""

# # Calculate percentage
# if [ $TOTAL_TESTS -gt 0 ]; then
#     PERCENTAGE=$((PASSED_TESTS * 100 / TOTAL_TESTS))
#     echo -e "Success rate: ${BOLD}$PERCENTAGE%${NC}"
#     echo ""
# fi

# # Final result
# if [ $FAILED_TESTS -eq 0 ]; then
#     echo -e "${GREEN}${BOLD}╔════════════════════════════════════════════════════════╗${NC}"
#     echo -e "${GREEN}${BOLD}║                                                        ║${NC}"
#     echo -e "${GREEN}${BOLD}║                  ✓ ALL TESTS PASSED!                   ║${NC}"
#     echo -e "${GREEN}${BOLD}║                                                        ║${NC}"
#     echo -e "${GREEN}${BOLD}╚════════════════════════════════════════════════════════╝${NC}"
#     rm -f /tmp/philo_test.log
#     exit 0
# else
#     echo -e "${RED}${BOLD}╔════════════════════════════════════════════════════════╗${NC}"
#     echo -e "${RED}${BOLD}║                                                        ║${NC}"
#     echo -e "${RED}${BOLD}║                  ✗ TESTS FAILED!                       ║${NC}"
#     echo -e "${RED}${BOLD}║                                                        ║${NC}"
#     echo -e "${RED}${BOLD}╚════════════════════════════════════════════════════════╝${NC}"
#     echo -e "${YELLOW}Check the output above for details.${NC}"
#     cat /tmp/philo_test.log > philo.log
#     rm -f /tmp/philo_test.
#     exit 1
# fi
