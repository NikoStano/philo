/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_loop.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nistanoj <nistanoj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 17:47:24 by nistanoj          #+#    #+#             */
/*   Updated: 2025/10/17 13:38:29 by nistanoj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/**
 * @brief Take forks for philosophers in small groups (<100)
 * @param [in] philo Pointer to the philosopher
 * @details
 * Odd ID philosophers pick up left fork first, even ID pick up right fork first.
 * This alternating strategy helps reduce contention and potential deadlocks.
 * @return 0 on success, 1 if simulation should stop
 */
static void	init_philosopher_time(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->meal_mutex);
	philo->last_meal_time = philo->data->start_time;
	pthread_mutex_unlock(&philo->data->meal_mutex);
}

/**
 * @brief Initial delay for philosophers to reduce contention
 * @param [in] philo Pointer to the philosopher
 * @details
 * Introduces a staggered start for philosophers based on their ID and
 * total count.
 * Helps to minimize simultaneous fork requests at the beginning of the
 * simulation.
 */
static void	philo_initial_delay(t_philo *philo)
{
	if (philo->data->nb_philos > 1)
	{
		if (philo->data->nb_philos >= 100)
			usleep((philo->id * 1000) % 5000);
		else if (philo->id % 2 == 0)
			usleep(100);
		else
			usleep(200);
	}
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	init_philosopher_time(philo);
	philo_initial_delay(philo);
	philo_loop(philo);
	return (NULL);
}
