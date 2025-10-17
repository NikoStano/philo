/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_actions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nistanoj <nistanoj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 18:08:34 by nistanoj          #+#    #+#             */
/*   Updated: 2025/10/17 12:45:12 by nistanoj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/**
 * @brief Handle the case of a single philosopher
 * @param [in] philo Pointer to the philosopher
 * @details
 * The single philosopher picks up the left fork, waits until they die,
 * and then releases the fork.
 * @return 1 to indicate the simulation should stop
 */
int	should_stop(t_philo *philo)
{
	int	stop;

	pthread_mutex_lock(&philo->data->stop_mutex);
	stop = philo->data->simulation_stop;
	pthread_mutex_unlock(&philo->data->stop_mutex);
	return (stop);
}

/**
 * @brief Philosopher eats
 * @param [in] philo Pointer to the philosopher
 * @details
 * Philosopher takes forks, updates last meal time and meals eaten,
 * sleeps for the eating duration, and then releases the forks.
 */
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

/**
 * @brief Philosopher thinks
 * @param [in] philo Pointer to the philosopher
 * @details
 * Philosopher prints thinking status and calculates think time based on
 * the number of philosophers and their eating/sleeping times.
 */
void	philo_think(t_philo *philo)
{
	long	think_time;

	print_status(philo, "is thinking");
	if (philo->data->nb_philos >= 100)
	{
		usleep(100);
		return ;
	}
	think_time = philo->data->time_to_die - philo->data->time_to_eat
			- philo->data->time_to_sleep;
	think_time = think_time / 2;
	if (think_time < 0)
		think_time = 0;
	else if (think_time > 1)
		think_time = 1;
	if (think_time > 0)
		precise_usleep(think_time * 1000);
}

/**
 * @brief Main loop for philosopher actions
 * @param [in] philo Pointer to the philosopher
 * @details
 * Continuously performs eating, sleeping, and thinking actions
 * until the simulation is signaled to stop.
 */
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
		if (should_stop(philo))
			break ;
		if (philo->data->nb_philos >= 100)
			usleep(500);
		else
			usleep(100);
	}
}
