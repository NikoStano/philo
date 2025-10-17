/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nistanoj <nistanoj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 14:04:40 by nistanoj          #+#    #+#             */
/*   Updated: 2025/10/17 10:53:14 by nistanoj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>

# define MAX_PHILOSOPHERS 200
# define PTHREAD_STACK_MIN 16384

/* ************************************************************************** */
/*                              ENUMERATIONS                                  */
/* ************************************************************************** */

/**
 * @enum e_state
 * @brief Enumeration for philosopher states
 *
 * @details
 * Represents the different states a philosopher can be in during the simulation.
 * Each state corresponds to a specific action or condition.
 */
typedef enum e_state
{
	THINKING,    /**< Philosopher is thinking */
	EATING,      /**< Philosopher is eating */
	SLEEPING,    /**< Philosopher is sleeping */
	DIED         /**< Philosopher has died */
}	t_state;

/* ************************************************************************** */
/*                              STRUCTURES                                    */
/* ************************************************************************** */

/**
 * @struct s_data
 * @brief Shared data structure for the simulation
 *
 * @details
 * Contains all global parameters and synchronization primitives shared
 * between all philosopher threads. This structure is initialized once
 * at the start of the simulation and accessed by all threads.
 */
typedef struct s_data
{
	int				nb_philos;           /**< Number of philosophers */
	long			time_to_die;         /**< Time to die in milliseconds */
	long			time_to_eat;         /**< Time to eat in milliseconds */
	long			time_to_sleep;       /**< Time to sleep in milliseconds */
	int				must_eat_count;      /**< Required meals (-1 if unlimited) */
	long			start_time;          /**< Simulation start timestamp */
	int				simulation_stop;     /**< Flag: 1 if simulation should stop */
	int				all_ate;             /**< Flag: 1 if all philos ate enough */
	pthread_mutex_t	print_mutex;         /**< Mutex for printing messages */
	pthread_mutex_t	stop_mutex;          /**< Mutex for simulation_stop access */
	pthread_mutex_t	meal_mutex;          /**< Mutex for last_meal_time access */
}	t_data;

/**
 * @struct s_philo
 * @brief Philosopher structure
 *
 * @details
 * Represents a single philosopher in the simulation. Each philosopher
 * runs in its own thread and maintains its own state, meal count,
 * and references to its assigned forks.
 */
typedef struct s_philo
{
	int				id;                  /**< Philosopher ID (1 to N) */
	int				meals_eaten;         /**< Number of meals consumed */
	long			last_meal_time;      /**< Timestamp of last meal start */
	t_state			state;               /**< Current philosopher state */
	pthread_t		thread;              /**< Thread handle */
	pthread_mutex_t	*left_fork;          /**< Pointer to left fork mutex */
	pthread_mutex_t	*right_fork;         /**< Pointer to right fork mutex */
	t_data			*data;               /**< Pointer to shared data */
}	t_philo;

/**
 * @struct s_simulation
 * @brief Main simulation structure containing all components
 *
 * @details
 * Top-level structure that encapsulates the entire simulation state.
 * Contains shared data, all philosophers, forks, and the monitor thread.
 */
typedef struct s_simulation
{
	t_data			data;                        /**< Shared simulation data */
	t_philo			philos[MAX_PHILOSOPHERS];    /**< Array of philosophers */
	pthread_mutex_t	forks[MAX_PHILOSOPHERS];     /**< Array of fork mutexes */
	pthread_t		monitor_thread;              /**< Monitor thread handle */
}	t_simulation;

/* ************************************************************************** */
/*                          FUNCTION PROTOTYPES                               */
/* ************************************************************************** */

/* --------------------------------- Thread --------------------------------- */

/**
 * @brief Creates and starts all philosopher threads
 * @param [in,out] sim Pointer to the simulation structure
 * @return 0 on success, 1 on failure
 *
 * @details
 * Creates one thread per philosopher and launches the simulation.
 * Each thread will execute the philosopher_routine() function.
 * Also creates the monitor thread to check for death and meal completion.
 */
int		create_philosophers(t_simulation *sim);

/* ---------------------------------- Init ---------------------------------- */

/**
 * @brief Initializes the simulation structure
 * @param [out] sim Pointer to the simulation structure to initialize
 * @param [in] argv Command-line arguments array
 * @return 0 on success, 1 on failure
 *
 * @details
 * Parses command-line arguments and initializes:
 * - Shared data structure
 * - All philosophers with their IDs and fork assignments
 * - All mutexes (forks, print, stop, meal)
 *
 * Expected arguments in argv:
 * - argv[1]: number_of_philosophers (1-200)
 * - argv[2]: time_to_die (ms)
 * - argv[3]: time_to_eat (ms)
 * - argv[4]: time_to_sleep (ms)
 * - argv[5]: [optional] number_of_times_each_philosopher_must_eat
 */
int		init_simulation(t_simulation *sim, char **argv);

/* --------------------------------- Parse ---------------------------------- */

/**
 * @brief Validates command-line arguments
 * @param [in] argc Argument count (must be 5 or 6)
 * @param [in] argv Argument vector
 * @return 0 if valid, 1 if invalid
 *
 * @details
 * Checks that:
 * - Correct number of arguments provided
 * - All arguments are valid positive integers
 * - Number of philosophers is within acceptable range (1-200)
 * - Time values are reasonable (> 0)
 */
int		validate_args(int argc, char **argv);

/* --------------------------------- Utils ---------------------------------- */

/**
 * @brief Converts a string to an integer
 * @param [in] str Input string to convert
 * @return Converted integer value
 *
 * @details
 * Custom implementation of atoi() for parsing command-line arguments.
 * Handles positive integers only.
 *
 * @warning Does not perform extensive error checking. Use is_number() first.
 */
int		ft_atoi(const char *str);

/**
 * @brief Checks if a string is a valid positive number
 * @param [in] str Input string to validate
 * @return 1 if valid number, 0 otherwise
 *
 * @details
 * Validates that the string:
 * - Contains only digits
 * - Represents a positive integer
 * - Has no leading/trailing whitespace
 */
int		is_number(const char *str);

/**
 * @brief Prints a philosopher's status message with timestamp
 * @param [in] philo Pointer to the philosopher
 * @param [in] status Status message to print
 *
 * @details
 * Thread-safe printing protected by print_mutex.
 * Format: "[timestamp_ms] philosopher_id status"
 * Example: "1234 2 is eating"
 *
 * Does not print if simulation has stopped.
 */
void	print_status(t_philo *philo, char *status);

/**
 * @brief Frees resources and destroys all mutexes
 * @param [in,out] sim Pointer to the simulation structure
 *
 * @details
 * Cleans up all allocated resources:
 * - Destroys all fork mutexes
 * - Destroys print, stop, and meal mutexes
 *
 * Should be called before program exit to prevent resource leaks.
 */
void	cleanup_simulation(t_simulation *sim);

/* -------------------------------- Routine --------------------------------- */

/**
 * @brief Attempts to take both forks for eating
 * @param [in,out] philo Pointer to the philosopher
 * @return 1 on success, 0 if simulation stopped
 *
 * @details
 * Locks both left and right fork mutexes in order to prevent deadlock.
 * For even-numbered philosophers: left fork first, then right fork.
 * For odd-numbered philosophers: right fork first, then left fork.
 *
 * @note This ordering prevents circular wait and thus deadlock.
 */
int		take_forks(t_philo *philo);

/* --------------------------- Routine Actions ------------------------------ */

/**
 * @brief Checks if the simulation should stop
 * @param [in] philo Pointer to the philosopher
 * @return 1 if simulation should stop, 0 otherwise
 *
 * @details
 * Thread-safe check protected by stop_mutex.
 * Returns true if any philosopher has died or all have eaten enough.
 */
int		should_stop(t_philo *philo);

/**
 * @brief Performs the eating action
 * @param [in,out] philo Pointer to the philosopher
 *
 * @details
 * Philosopher eats for time_to_eat milliseconds.
 * Updates:
 * - last_meal_time to current timestamp
 * - meals_eaten counter
 * - state to EATING
 * Prints "is eating" status message.
 */
void	philo_eat(t_philo *philo);

/**
 * @brief Performs the sleeping action
 * @param [in,out] philo Pointer to the philosopher
 *
 * @details
 * Philosopher sleeps for time_to_sleep milliseconds.
 * Updates state to SLEEPING.
 * Prints "is sleeping" status message.
 */
void	philo_sleep(t_philo *philo);

/**
 * @brief Performs the thinking action
 * @param [in,out] philo Pointer to the philosopher
 *
 * @details
 * Updates state to THINKING.
 * Prints "is thinking" status message.
 * May include a small delay to prevent busy-waiting.
 */
void	philo_think(t_philo *philo);

/**
 * @brief Main loop for philosopher thread
 * @param [in,out] philo Pointer to the philosopher
 *
 * @details
 * Executes the philosopher's life cycle:
 * 1. Think
 * 2. Take forks
 * 3. Eat
 * 4. Release forks
 * 5. Sleep
 * 6. Repeat until death or meal requirement met
 */
void	philo_loop(t_philo *philo);

/* ----------------------------- Routine Loop ------------------------------- */

/**
 * @brief Main routine executed by each philosopher thread
 * @param [in] arg Pointer to philosopher structure (cast from void*)
 * @return NULL (required by pthread API)
 *
 * @details
 * Entry point for philosopher threads.
 * Executes the philosopher's life cycle until:
 * - The philosopher dies
 * - All philosophers have eaten enough
 * - Another philosopher dies
 */
void	*philosopher_routine(void *arg);

/* -------------------------------- Monitor --------------------------------- */

/**
 * @brief Monitor thread routine
 * @param [in] arg Pointer to simulation structure (cast from void*)
 * @return NULL (required by pthread API)
 *
 * @details
 * Continuously monitors all philosophers to detect:
 * - Death (time since last meal exceeds time_to_die)
 * - Meal completion (all philos have eaten must_eat_count times)
 *
 * Sets simulation_stop flag when a stopping condition is met.
 */
void	*monitor_routine(void *arg);

/**
 * @brief Checks if any philosopher has died
 * @param [in,out] sim Pointer to the simulation structure
 * @return 1 if a philosopher died, 0 otherwise
 *
 * @details
 * Iterates through all philosophers and checks if:
 * (current_time - last_meal_time) > time_to_die
 *
 * If death detected:
 * - Prints death message
 * - Sets simulation_stop flag
 * - Returns immediately
 */
int		check_death(t_simulation *sim);

/**
 * @brief Checks if all philosophers have eaten enough
 * @param [in,out] sim Pointer to the simulation structure
 * @return 1 if all ate enough, 0 otherwise
 *
 * @details
 * Only relevant if must_eat_count is specified (> 0).
 * Checks if all philosophers have eaten at least must_eat_count meals.
 *
 * If condition met:
 * - Sets all_ate and simulation_stop flags
 * - Simulation ends gracefully
 */
int		check_all_ate(t_simulation *sim);

/* --------------------------------- Time ----------------------------------- */

/**
 * @brief Gets current timestamp in milliseconds
 * @return Current time in milliseconds since Epoch
 *
 * @details
 * Uses gettimeofday() to get high-precision timestamp.
 * Converts timeval structure to milliseconds.
 *
 * @note Used for all timing operations in the simulation.
 */
long	get_current_time(void);

/**
 * @brief Precise sleep function with microsecond accuracy
 * @param [in] usec Duration to sleep in microseconds
 *
 * @details
 * More accurate than standard usleep() for short durations.
 * Uses usleep() in small intervals to allow responsive interruption.
 * Checks simulation_stop frequently for graceful shutdown.
 */
void	precise_usleep(long usec);

/**
 * @brief Calculates time difference between two timestamps
 * @param [in] start Start timestamp in milliseconds
 * @param [in] end End timestamp in milliseconds
 * @return Time difference (end - start) in milliseconds
 *
 * @details
 * Utility function for calculating elapsed time.
 * Used for checking if time_to_die has been exceeded.
 */
long	time_diff(long start, long end);

#endif