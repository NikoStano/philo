/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nistanoj <nistanoj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 17:47:24 by nistanoj          #+#    #+#             */
/*   Updated: 2025/10/22 19:03:49 by nistanoj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/philo.h"

static void	init_philosopher_time(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->meal_mutex);
	philo->last_meal_time = philo->data->start_time;
	pthread_mutex_unlock(&philo->data->meal_mutex);
}

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
