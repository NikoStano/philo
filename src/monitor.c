/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nistanoj <nistanoj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 14:07:23 by nistanoj          #+#    #+#             */
/*   Updated: 2025/10/17 15:03:52 by nistanoj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/**
 * @brief Check if any philosopher has died
 * @param [in] sim Pointer to the simulation
 * @return 1 if a philosopher has died, 0 otherwise
 * @details
 * Iterates through all philosophers and checks if the time since their last meal
 * exceeds the time_to_die threshold. If a philosopher is found to have died,
 * it sets the simulation_stop flag and prints the death status.
 */
static int	get_check_interval(int nb_philos)
{
	if (nb_philos >= 100)
		return (2000);
	else if (nb_philos >= 50)
		return (1000);
	else
		return (500);
}

static void	initial_delay(int nb_philos)
{
	if (nb_philos >= 100)
		usleep(10000);
	else
		usleep(1000);
}

/**
 * @brief Monitor routine to check for philosopher deaths and meal completion
 * @param [in] arg Pointer to the simulation structure
 * @return NULL
 * @details
 * Continuously checks if any philosopher has died or if all philosophers
 * have eaten the required number of meals. If either condition is met,
 * it sets the simulation_stop flag and exits the monitoring loop.
 */
void	*monitor_routine(void *arg)
{
	t_simulation	*sim;
	int				check_interval;

	sim = (t_simulation *)arg;
	check_interval = get_check_interval(sim->data.nb_philos);
	initial_delay(sim->data.nb_philos);
	while (1)
	{
		pthread_mutex_lock(&sim->data.stop_mutex);
		if (sim->data.simulation_stop)
		{
			pthread_mutex_unlock(&sim->data.stop_mutex);
			break ;
		}
		pthread_mutex_unlock(&sim->data.stop_mutex);
		if (check_death(sim) || check_all_ate(sim))
			break ;
		usleep(check_interval);
	}
	return (NULL);
}
