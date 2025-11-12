/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_actions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nistanoj <nistanoj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 18:08:34 by nistanoj          #+#    #+#             */
/*   Updated: 2025/11/11 04:11:55 by nistanoj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/philo.h"

static int	check_meals_finished(t_philo *philo)
{
	int	finished;

	finished = 0;
	if (philo->data->must_eat_count != -1)
	{
		pthread_mutex_lock(&philo->data->meal_mutex);
		if (philo->meals_eaten >= philo->data->must_eat_count)
			finished = 1;
		pthread_mutex_unlock(&philo->data->meal_mutex);
	}
	return (finished);
}

void	philo_eat(t_philo *philo)
{
	if (take_forks(philo))
		return ;
	pthread_mutex_lock(&philo->data->meal_mutex);
	philo->last_meal_time = get_current_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->meal_mutex);
	print_status(philo, "is eating");
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
	long	margin;

	print_status(philo, "is thinking");
	if (philo->data->nb_philos == 1)
		return ;
	margin = philo->data->time_to_die - philo->data->time_to_eat
		- philo->data->time_to_sleep;
	if (philo->data->nb_philos % 2 != 0)
	{
		think_time = calculate_odd_think_time(philo, margin);
		if (think_time > 0)
			precise_usleep(think_time * 1000);
	}
	else if (margin < 50)
	{
		think_time = (philo->data->time_to_eat / philo->data->nb_philos)
			* (philo->id % 3);
		if (think_time > 0 && think_time < margin)
			usleep(think_time * 100);
	}
}

void	philo_loop(t_philo *philo)
{
	int	finished;

	finished = 0;
	while (!should_stop(philo))
	{
		if (!finished)
			philo_eat(philo);
		if (should_stop(philo))
			break ;
		finished = check_meals_finished(philo);
		if (!finished)
		{
			philo_sleep(philo);
			if (should_stop(philo))
				break ;
			philo_think(philo);
		}
		else
			usleep(1000);
	}
}
