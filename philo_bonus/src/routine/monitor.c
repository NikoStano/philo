/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nistanoj <nistanoj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 22:30:00 by nistanoj          #+#    #+#             */
/*   Updated: 2025/10/27 02:49:32 by nistanoj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/philo_bonus.h"

static int	check_philosopher_death(t_philo *philo)
{
	long	current_time;
	long	last_meal;

	sem_wait(philo->data->meal_sem);
	current_time = get_current_time();
	last_meal = philo->last_meal_time;
	sem_post(philo->data->meal_sem);
	if (current_time - last_meal >= philo->data->time_to_die)
	{
		sem_wait(philo->data->print_sem);
		sem_wait(philo->data->dead_sem);
		philo->data->simulation_stop = 1;
		printf("%ld %d died\n", current_time - philo->data->start_time,
			philo->id);
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_philo	*philo;
	int		check_interval;

	philo = (t_philo *)arg;
	if (philo->data->time_to_die < 50)
		check_interval = 100;
	else if (philo->data->time_to_die < 100)
		check_interval = 200;
	else if (philo->data->time_to_die < 200)
		check_interval = 500;
	else
		check_interval = 1000;
	usleep(1000);
	while (1)
	{
		if (check_philosopher_death(philo))
			exit(1);
		usleep(check_interval);
	}
	return (NULL);
}
