/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_actions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nistanoj <nistanoj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 22:30:00 by nistanoj          #+#    #+#             */
/*   Updated: 2025/10/30 17:06:23 by nistanoj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/philo_bonus.h"

void	philo_eat(t_philo *philo)
{
	sem_wait(philo->data->forks);
	print_status(philo, "has taken a fork");
	sem_wait(philo->data->forks);
	print_status(philo, "has taken a fork");
	sem_wait(philo->data->meal_sem);
	philo->last_meal_time = get_current_time();
	philo->meals_eaten++;
	sem_post(philo->data->meal_sem);
	print_status(philo, "is eating");
	precise_usleep(philo->data->time_to_eat * 1000);
	sem_post(philo->data->forks);
	sem_post(philo->data->forks);
}

void	philo_sleep(t_philo *philo)
{
	print_status(philo, "is sleeping");
	precise_usleep(philo->data->time_to_sleep * 1000);
}

void	philo_think(t_philo *philo)
{
	long	think_time;
	long	safety_margin;

	print_status(philo, "is thinking");
	if (philo->data->nb_philos == 1)
		return ;
	safety_margin = philo->data->time_to_die - philo->data->time_to_eat
		- philo->data->time_to_sleep;
	if (philo->data->nb_philos % 2 != 0)
	{
		think_time = calculate_odd_think_time(philo, safety_margin);
		if (think_time > 0)
			precise_usleep(think_time * 1000);
	}
	else if (safety_margin < 50)
	{
		think_time = (philo->data->time_to_eat / philo->data->nb_philos)
			* (philo->id % 3);
		if (think_time > 0 && think_time < safety_margin)
			usleep(think_time * 100);
	}
}

void	philo_loop(t_philo *philo)
{
	while (!should_stop(philo))
	{
		philo_eat(philo);
		if (!should_continue(philo))
			break ;
		philo_sleep(philo);
		if (should_stop(philo))
			break ;
		philo_think(philo);
	}
}
