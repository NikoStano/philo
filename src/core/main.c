/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nistanoj <nistanoj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 14:05:26 by nistanoj          #+#    #+#             */
/*   Updated: 2025/10/17 11:04:02 by nistanoj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/**
 * @brief Join all philosopher threads
 * @param [in] sim Pointer to the simulation
 * @return 0 on success, 1 on failure
 * @details
 * Waits for all philosopher threads to finish execution.
 * If joining any thread fails, an error message is printed.
 * Cleans up resources should be handled by the caller.
 */
static int	join_philosophers(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->data.nb_philos)
	{
		if (pthread_join(sim->philos[i].thread, NULL) != 0)
		{
			printf("Error: Failed to join philosopher thread %d\n", i);
			return (1);
		}
		i++;
	}
	return (0);
}

/**
 * @brief Start the simulation
 * @param [in] sim Pointer to the simulation structure
 * @return 0 on success, 1 on failure
 * @details
 * Initializes the start time, creates philosopher threads,
 * starts the monitor thread, and waits for all threads to finish.
 * Cleans up resources on failure.
 */
static int	start_simulation(t_simulation *sim)
{
	sim->data.start_time = get_current_time();
	if (create_philosophers(sim) != 0)
		return (1);
	if (pthread_create(&sim->monitor_thread, NULL, monitor_routine, sim) != 0)
	{
		printf("Error: Failed to create monitor thread\n");
		return (1);
	}
	if (pthread_join(sim->monitor_thread, NULL) != 0)
	{
		printf("Error: Failed to join monitor thread\n");
		return (1);
	}
	if (join_philosophers(sim) != 0)
		return (1);
	return (0);
}

int	main(int argc, char **argv)
{
	t_simulation	sim;

	if (validate_args(argc, argv) != 0)
		return (1);
	if (init_simulation(&sim, argv) != 0)
		return (1);
	if (start_simulation(&sim) != 0)
		return (cleanup_simulation(&sim), 1);
	cleanup_simulation(&sim);
	return (0);
}
