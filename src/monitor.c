/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nistanoj <nistanoj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 14:07:23 by nistanoj          #+#    #+#             */
/*   Updated: 2025/10/07 17:26:46 by nistanoj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
		pthread_mutex_lock(&philo->data->stop_mutex);
		philo->data->simulation_stop = 1;
		pthread_mutex_unlock(&philo->data->stop_mutex);
		pthread_mutex_lock(&philo->data->print_mutex);
		current_time = get_current_time() - philo->data->start_time;
		printf("%ld %d died\n", current_time, philo->id);
		pthread_mutex_unlock(&philo->data->print_mutex);
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

void	*monitor_routine(void *arg)
{
	t_simulation	*sim;
	int				check_interval;

	sim = (t_simulation *)arg;
	if (sim->data.nb_philos >= 100)
		check_interval = 2000;
	else if (sim->data.nb_philos >= 50)
		check_interval = 1000;
	else
		check_interval = 500;
	if (sim->data.nb_philos >= 100)
		usleep(10000);
	else
		usleep(1000);
	while (1)
	{
		pthread_mutex_lock(&sim->data.stop_mutex);
		if (sim->data.simulation_stop)
		{
			pthread_mutex_unlock(&sim->data.stop_mutex);
			break ;
		}
		pthread_mutex_unlock(&sim->data.stop_mutex);
		if (check_death(sim) || check_all_ate(sim))
			break ;
		usleep(check_interval);
	}
	return (NULL);
}
