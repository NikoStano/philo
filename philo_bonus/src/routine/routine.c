/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nistanoj <nistanoj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 22:30:00 by nistanoj          #+#    #+#             */
/*   Updated: 2025/10/27 03:08:26 by nistanoj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/philo_bonus.h"

static void	init_philosopher_time(t_philo *philo)
{
	sem_wait(philo->data->meal_sem);
	philo->last_meal_time = philo->data->start_time;
	sem_post(philo->data->meal_sem);
}

static void	philo_initial_delay(t_philo *philo)
{
	if (philo->data->nb_philos > 1)
	{
		if (philo->data->nb_philos >= 100)
			usleep((philo->id * 1000) % 5000);
		else if (philo->data->nb_philos % 2 != 0)
		{
			if (philo->id % 2 == 0)
				precise_usleep((philo->data->time_to_eat * 900));
			else if (philo->id == philo->data->nb_philos)
				precise_usleep((philo->data->time_to_eat * 1000
						+ philo->data->time_to_sleep * 500));
		}
		else if (philo->id % 2 == 0)
			precise_usleep(1000);
	}
}

void	philosopher_routine(t_philo *philo)
{
	init_philosopher_time(philo);
	philo_initial_delay(philo);
	if (philo->data->nb_philos == 1)
	{
		print_status(philo, "has taken a fork");
		precise_usleep(philo->data->time_to_die * 1000);
		sem_wait(philo->data->dead_sem);
		philo->data->simulation_stop = 1;
		sem_wait(philo->data->print_sem);
		printf("%ld %d died\n", get_current_time() - philo->data->start_time,
			philo->id);
		sem_post(philo->data->print_sem);
		sem_post(philo->data->dead_sem);
		exit(1);
	}
	if (pthread_create(&philo->monitor_thread, NULL, monitor_routine,
			philo) != 0)
	{
		printf("Error: Failed to create monitor thread\n");
		exit(1);
	}
	pthread_detach(philo->monitor_thread);
	philo_loop(philo);
	exit(0);
}
