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

# Function to run a test
run_test() {
    local test_name="$1"
    local expected_result="$2"  # "success" or "death" or "fail"
    shift 2
    local args="$@"

    TOTAL_TESTS=$((TOTAL_TESTS + 1))

    echo -ne "${YELLOW}Test $TOTAL_TESTS: $test_name${NC}"

    # Run the test with timeout
    timeout 15 $PHILO $args > /tmp/philo_test.log 2>&1
    local exit_code=$?

    # Check result based on expected outcome
    local test_passed=0

    case $expected_result in
        "success")
            # Program should exit normally (0) or timeout means it's still running (good)
            if [ $exit_code -eq 0 ] || [ $exit_code -eq 124 ]; then
                # Check for no errors in output
                if ! grep -qi "error" /tmp/philo_test.log; then
                    test_passed=1
                fi
            fi
            ;;
        "death")
            # Program should detect death (can exit with 0 or be killed by timeout)
            if grep -q "died" /tmp/philo_test.log; then
                test_passed=1
            fi
            ;;
        "fail")
            # Program should fail (non-zero exit and error message)
            if [ $exit_code -ne 0 ] && [ $exit_code -ne 124 ]; then
                test_passed=1
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
        echo -e "${RED}   Exit code: $exit_code${NC}"
    fi

    sleep 0.5
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

run_test "No arguments" "fail"
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

run_test "Single philosopher (must die)" "death" 1 800 200 200
run_test "Two philosophers (should die)" "death" 2 400 200 200
run_test "Should die (5 800 200 200)" "death" 5 800 200 200
run_test "Quick death (4 310 200 100)" "death" 4 310 200 100
run_test "Very quick death (3 200 100 100)" "death" 3 200 100 100
run_test "Instant death (4 100 50 50)" "death" 4 100 50 50

echo ""

# ============================================================================
# NO DEATH TESTS (with meal limit)
# ============================================================================
echo -e "${GREEN}${BOLD}[3] NO DEATH TESTS (with meal limit)${NC}"
echo "-----------------------------------------------------------"

run_test "No death (5 800 200 200 7)" "success" 5 800 200 200 7
run_test "Tight timing (4 410 200 200 5)" "success" 4 410 200 200 5
run_test "Safe timing (3 800 200 200 10)" "success" 3 800 200 200 10
run_test "Many meals (4 800 200 200 50)" "success" 4 800 200 200 50

echo ""

# ============================================================================
# STRESS TESTS
# ============================================================================
echo -e "${BLUE}${BOLD}[4] STRESS TESTS${NC}"
echo "-----------------------------------------------------------"

run_test "Many philosophers (100 800 200 200 7)" "success" 100 800 200 200 7
run_test "Many philosophers (200 800 200 200 7)" "success" 200 800 200 200 7
run_test "Long simulation (10 800 200 200 100)" "success" 10 800 200 200 100
run_test "High concurrency (50 410 200 200 10)" "success" 50 410 200 200 10

echo ""

# ============================================================================
# TIMING TESTS
# ============================================================================
echo -e "${CYAN}${BOLD}[5] TIMING PRECISION TESTS${NC}"
echo "-----------------------------------------------------------"

run_test "Minimum safe time (4 410 200 200 5)" "success" 4 410 200 200 5
run_test "Just enough time (5 400 100 100 7)" "success" 5 400 100 100 7
run_test "Fast eating (10 600 100 100 7)" "success" 10 600 100 100 7
run_test "Fast sleeping (10 600 200 50 7)" "success" 10 600 200 50 7

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
    echo -e "${YELLOW}Check the output above for details.${NC}"
    cat /tmp/philo_test.log > philo.log
    rm -f /tmp/philo_test.
    exit 1
fi
