/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_forks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nistanoj <nistanoj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 14:06:20 by nistanoj          #+#    #+#             */
/*   Updated: 2025/11/11 01:35:05 by nistanoj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/philo.h"

static int	handle_single_philo(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_status(philo, "has taken a fork");
	precise_usleep(philo->data->time_to_die * 1000);
	pthread_mutex_unlock(philo->left_fork);
	return (1);
}

static int	lock_first_fork(t_philo *philo, pthread_mutex_t *first)
{
	pthread_mutex_lock(first);
	print_status(philo, "has taken a fork");
	if (should_stop(philo))
	{
		pthread_mutex_unlock(first);
		return (1);
	}
	return (0);
}

static int	take_forks_large(t_philo *philo)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	if (philo->left_fork < philo->right_fork)
	{
		first = philo->left_fork;
		second = philo->right_fork;
	}
	else
	{
		first = philo->right_fork;
		second = philo->left_fork;
	}
	if (lock_first_fork(philo, first))
		return (1);
	pthread_mutex_lock(second);
	print_status(philo, "has taken a fork");
	return (0);
}

static int	take_forks_small(t_philo *philo)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	if (philo->id % 2 == 1)
	{
		first = philo->left_fork;
		second = philo->right_fork;
	}
	else
	{
		first = philo->right_fork;
		second = philo->left_fork;
	}
	if (lock_first_fork(philo, first))
		return (1);
	pthread_mutex_lock(second);
	print_status(philo, "has taken a fork");
	return (0);
}

int	take_forks(t_philo *philo)
{
	if (philo->data->nb_philos == 1)
		return (handle_single_philo(philo));
	if (philo->data->nb_philos >= 100)
		return (take_forks_large(philo));
	return (take_forks_small(philo));
}
