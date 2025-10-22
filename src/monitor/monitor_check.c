/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_check.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nistanoj <nistanoj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 18:31:56 by nistanoj          #+#    #+#             */
/*   Updated: 2025/10/22 19:03:30 by nistanoj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/philo.h"

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

static int	philosopher_died(t_philo *philo)
{
	long	current_time;
	long	last_meal;

	pthread_mutex_lock(&philo->data->meal_mutex);
	current_time = get_current_time();
	last_meal = philo->last_meal_time;
	pthread_mutex_unlock(&philo->data->meal_mutex);
	if (current_time - last_meal >= philo->data->time_to_die)
	{
		announce_death(philo);
		return (1);
	}
	return (0);
}

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
