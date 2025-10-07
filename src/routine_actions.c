/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_actions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nistanoj <nistanoj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 18:08:34 by nistanoj          #+#    #+#             */
/*   Updated: 2025/10/07 18:24:18 by nistanoj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	should_stop(t_philo *philo)
{
	int	stop;

	pthread_mutex_lock(&philo->data->stop_mutex);
	stop = philo->data->simulation_stop;
	pthread_mutex_unlock(&philo->data->stop_mutex);
	return (stop);
}

void	philo_eat(t_philo *philo)
{
	if (take_forks(philo) != 0)
		return ;
	print_status(philo, "is eating");
	pthread_mutex_lock(&philo->data->meal_mutex);
	philo->last_meal_time = get_current_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->meal_mutex);
	precise_usleep(philo->data->time_to_eat * 1000);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	philo_sleep(t_philo *philo)
{
	print_status(philo, "is sleeping");
	precise_usleep(philo->data->time_to_sleep * 1000);
}

void	philo_think(t_philo *philo)
{
	long	think_time;

	print_status(philo, "is thinking");
	if (philo->data->nb_philos >= 100)
		think_time = 1;
	else
	{
		think_time = (philo->data->time_to_eat * 2) \
		- philo->data->time_to_sleep;
		if (think_time < 0 || think_time > 600)
			think_time = 1;
	}
	if (think_time > 0)
		precise_usleep(think_time * 1000);
}

void	philo_loop(t_philo *philo)
{
	while (1)
	{
		if (should_stop(philo))
			break ;
		philo_eat(philo);
		if (should_stop(philo))
			break ;
		philo_sleep(philo);
		if (should_stop(philo))
			break ;
		philo_think(philo);
		if (philo->data->nb_philos >= 100)
			usleep(500);
		else
			usleep(100);
	}
}
