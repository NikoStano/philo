/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_check.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nistanoj <nistanoj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 18:31:56 by nistanoj          #+#    #+#             */
/*   Updated: 2025/10/17 11:03:04 by nistanoj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/**
 * @brief Announce the death of a philosopher
 * @param [in] philo Pointer to the philosopher
 * @details
 * Sets the simulation_stop flag and prints the death message with timestamp.
 */
static void	announce_death(t_philo *philo)
{
	long	current_time;

	pthread_mutex_lock(&philo->data->stop_mutex);
	philo->data->simulation_stop = 1;
	pthread_mutex_unlock(&philo->data->stop_mutex);
	pthread_mutex_lock(&philo->data->print_mutex);
	current_time = get_current_time() - philo->data->start_time;
	printf("%ld %d died\n", current_time, philo->id);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

/**
 * @brief Check if a philosopher has died
 * @param [in] philo Pointer to the philosopher
 * @return 1 if the philosopher has died, 0 otherwise
 */
static int	philosopher_died(t_philo *philo)
{
	long	current_time;
	long	last_meal;

	pthread_mutex_lock(&philo->data->meal_mutex);
	current_time = get_current_time();
	last_meal = philo->last_meal_time;
	pthread_mutex_unlock(&philo->data->meal_mutex);
	if (current_time - last_meal > philo->data->time_to_die)
	{
		announce_death(philo);
		return (1);
	}
	return (0);
}

/**
 * @brief Check if any philosopher has died
 * @param [in] sim Pointer to the simulation
 * @return 1 if a philosopher has died, 0 otherwise
 * @details
 * Iterates through all philosophers and checks if the time since their last meal
 * exceeds the time_to_die threshold. If a philosopher is found to have died,
 * it sets the simulation_stop flag and prints the death status.
 */
int	check_death(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->data.nb_philos)
	{
		if (philosopher_died(&sim->philos[i]))
			return (1);
		i++;
	}
	return (0);
}

/**
 * @brief Check if all philosophers have eaten the required number of meals
 * @param [in] sim Pointer to the simulation
 * @return 1 if all philosophers have eaten enough, 0 otherwise
 * @details
 * If must_eat_count is -1, this check is skipped.
 * Iterates through all philosophers and counts how many have eaten
 * at least must_eat_count meals. If all have, sets simulation_stop and all_ate flags.
 */
int	check_all_ate(t_simulation *sim)
{
	int	i;
	int	finished_eating;

	if (sim->data.must_eat_count == -1)
		return (0);
	finished_eating = 0;
	i = 0;
	while (i < sim->data.nb_philos)
	{
		pthread_mutex_lock(&sim->data.meal_mutex);
		if (sim->philos[i].meals_eaten >= sim->data.must_eat_count)
			finished_eating++;
		pthread_mutex_unlock(&sim->data.meal_mutex);
		i++;
	}
	if (finished_eating == sim->data.nb_philos)
	{
		pthread_mutex_lock(&sim->data.stop_mutex);
		sim->data.simulation_stop = 1;
		sim->data.all_ate = 1;
		pthread_mutex_unlock(&sim->data.stop_mutex);
		return (1);
	}
	return (0);
}
