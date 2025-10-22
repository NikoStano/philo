/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nistanoj <nistanoj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 14:05:46 by nistanoj          #+#    #+#             */
/*   Updated: 2025/10/19 18:47:13 by nistanoj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/**
 * @brief Initialize mutexes for the simulation
 * @param [in] sim Pointer to the simulation
 * @return 0 on success, 1 on failure
 * @details
 * Initializes mutexes for printing, stopping the simulation,
 * meal time access, and each fork.
 * If any mutex initialization fails, returns 1.
 */
static int	init_mutexes(t_simulation *sim)
{
	int	i;

	if (pthread_mutex_init(&sim->data.print_mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&sim->data.stop_mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&sim->data.meal_mutex, NULL) != 0)
		return (1);
	i = 0;
	while (i < sim->data.nb_philos)
	{
		if (pthread_mutex_init(&sim->forks[i], NULL) != 0)
			return (1);
		i++;
	}
	return (0);
}

/**
 * @brief Initialize philosophers in the simulation
 * @param [in] sim Pointer to the simulation
 * @details
 * Sets up each philosopher's ID, meal count, last meal time,
 * state, data pointer, and fork pointers.
 */
static void	init_philosophers(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->data.nb_philos)
	{
		sim->philos[i].id = i + 1;
		sim->philos[i].meals_eaten = 0;
		sim->philos[i].last_meal_time = 0;
		sim->philos[i].state = THINKING;
		sim->philos[i].data = &sim->data;
		sim->philos[i].left_fork = &sim->forks[i];
		if (sim->data.nb_philos == 1)
			sim->philos[i].right_fork = &sim->forks[i];
		else
			sim->philos[i].right_fork = \
&sim->forks[(i + 1) % sim->data.nb_philos];
		i++;
	}
}

/**
 * @brief Initialize the simulation structure
 * @param [in] sim Pointer to the simulation
 * @param [in] argv Argument vector
 * @return 0 on success, 1 on failure
 * @details
 * Parses command-line arguments to set simulation parameters,
 * initializes mutexes and philosophers.
 */
int	init_simulation(t_simulation *sim, char **argv)
{
	memset(sim, 0, sizeof(t_simulation));
	sim->data.nb_philos = ft_atoi(argv[1]);
	sim->data.time_to_die = (long)ft_atoi(argv[2]);
	sim->data.time_to_eat = (long)ft_atoi(argv[3]);
	sim->data.time_to_sleep = (long)ft_atoi(argv[4]);
	sim->data.must_eat_count = -1;
	sim->data.simulation_stop = 0;
	sim->data.all_ate = 0;
	if (argv[5])
		sim->data.must_eat_count = ft_atoi(argv[5]);
	if (init_mutexes(sim) != 0)
	{
		printf("Error: Failed to initialize mutexes\n");
		return (1);
	}
	init_philosophers(sim);
	return (0);
}
