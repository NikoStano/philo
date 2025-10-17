/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nistanoj <nistanoj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 14:06:20 by nistanoj          #+#    #+#             */
/*   Updated: 2025/10/17 10:58:43 by nistanoj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static int	handle_single_philo(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_status(philo, "has taken a fork");
	precise_usleep(philo->data->time_to_die * 1000);
	pthread_mutex_unlock(philo->left_fork);
	return (1);
}

/**
 * @brief Lock the first fork and print status
 * @param [in] philo Pointer to the philosopher
 * @param [in] first Pointer to the first fork mutex
 * @details
 * Locks the specified fork mutex and prints the status.
 * Checks if the simulation should stop after locking.
 * @return 1 if simulation should stop after locking, 0 otherwise
 */
int	lock_first_fork(t_philo *philo, pthread_mutex_t *first)
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

/**
 * @brief Take forks for philosophers in large groups (>=100)
 * @param [in] philo Pointer to the philosopher
 * @details
 * Philosophers pick up the lower-addressed fork first to prevent deadlock.
 * This strategy helps avoid circular wait conditions.
 * @return 0 on success, 1 if simulation should stop
 */
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

/**
 * @brief Take forks for philosophers in small groups (<100)
 * @param [in] philo Pointer to the philosopher
 * @details
 * Odd ID philosophers pick up left fork first, even ID pick up right fork first.
 * This alternating strategy helps reduce contention and potential deadlocks.
 * @return 0 on success, 1 if simulation should stop
 */
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

/**
 * @brief Take forks for the philosopher
 * @param [in] philo Pointer to the philosopher
 * @details
 * Chooses the fork-taking strategy based on the number of philosophers.
 * Uses a specialized strategy for single philosopher, large groups, and small groups.
 * @return 0 on success, 1 if simulation should stop
 */
int	take_forks(t_philo *philo)
{
	if (philo->data->nb_philos == 1)
		return (handle_single_philo(philo));
	if (philo->data->nb_philos >= 100)
		return (take_forks_large(philo));
	return (take_forks_small(philo));
}
