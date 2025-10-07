/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nistanoj <nistanoj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 14:06:20 by nistanoj          #+#    #+#             */
/*   Updated: 2025/10/07 17:32:46 by nistanoj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	take_forks(t_philo *philo)
{
	if (philo->data->nb_philos == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		precise_usleep(philo->data->time_to_die * 1000);
		pthread_mutex_unlock(philo->left_fork);
		return (1);
	}
	if (philo->data->nb_philos >= 100)
	{
		if (philo->left_fork < philo->right_fork)
		{
			pthread_mutex_lock(philo->left_fork);
			print_status(philo, "has taken a fork");
			pthread_mutex_lock(&philo->data->stop_mutex);
			if (philo->data->simulation_stop)
			{
				pthread_mutex_unlock(&philo->data->stop_mutex);
				pthread_mutex_unlock(philo->left_fork);
				return (1);
			}
			pthread_mutex_unlock(&philo->data->stop_mutex);
			pthread_mutex_lock(philo->right_fork);
			print_status(philo, "has taken a fork");
		}
		else
		{
			pthread_mutex_lock(philo->right_fork);
			print_status(philo, "has taken a fork");
			pthread_mutex_lock(&philo->data->stop_mutex);
			if (philo->data->simulation_stop)
			{
				pthread_mutex_unlock(&philo->data->stop_mutex);
				pthread_mutex_unlock(philo->right_fork);
				return (1);
			}
			pthread_mutex_unlock(&philo->data->stop_mutex);
			pthread_mutex_lock(philo->left_fork);
			print_status(philo, "has taken a fork");
		}
	}
	else
	{
		if (philo->id % 2 == 1)
		{
			pthread_mutex_lock(philo->left_fork);
			print_status(philo, "has taken a fork");
			pthread_mutex_lock(&philo->data->stop_mutex);
			if (philo->data->simulation_stop)
			{
				pthread_mutex_unlock(&philo->data->stop_mutex);
				pthread_mutex_unlock(philo->left_fork);
				return (1);
			}
			pthread_mutex_unlock(&philo->data->stop_mutex);
			pthread_mutex_lock(philo->right_fork);
			print_status(philo, "has taken a fork");
		}
		else
		{
			pthread_mutex_lock(philo->right_fork);
			print_status(philo, "has taken a fork");
			pthread_mutex_lock(&philo->data->stop_mutex);
			if (philo->data->simulation_stop)
			{
				pthread_mutex_unlock(&philo->data->stop_mutex);
				pthread_mutex_unlock(philo->right_fork);
				return (1);
			}
			pthread_mutex_unlock(&philo->data->stop_mutex);
			pthread_mutex_lock(philo->left_fork);
			print_status(philo, "has taken a fork");
		}
	}
	return (0);
}

static void	drop_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
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
	drop_forks(philo);
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

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;
	long	delay;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->data->meal_mutex);
	philo->last_meal_time = philo->data->start_time;
	pthread_mutex_unlock(&philo->data->meal_mutex);
	if (philo->data->nb_philos > 1)
	{
		if (philo->data->nb_philos >= 100)
			usleep((philo->id * 1000) % 5000);
		else if (philo->id % 2 == 0)
		{
			delay = (philo->data->time_to_eat * 800) / 1000;
			if (delay > 0)
				precise_usleep(delay);
		}
		else
			usleep(200);
	}
	while (1)
	{
		pthread_mutex_lock(&philo->data->stop_mutex);
		if (philo->data->simulation_stop)
		{
			pthread_mutex_unlock(&philo->data->stop_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->data->stop_mutex);
		philo_eat(philo);
		pthread_mutex_lock(&philo->data->stop_mutex);
		if (philo->data->simulation_stop)
		{
			pthread_mutex_unlock(&philo->data->stop_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->data->stop_mutex);
		philo_sleep(philo);
		pthread_mutex_lock(&philo->data->stop_mutex);
		if (philo->data->simulation_stop)
		{
			pthread_mutex_unlock(&philo->data->stop_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->data->stop_mutex);
		philo_think(philo);
		if (philo->data->nb_philos >= 100)
			usleep(500);
		else
			usleep(100);
	}
	return (NULL);
}
