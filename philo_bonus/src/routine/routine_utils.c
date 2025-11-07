/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nistanoj <nistanoj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:00:00 by nistanoj          #+#    #+#             */
/*   Updated: 2025/10/30 17:03:06 by nistanoj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/philo_bonus.h"

int	should_stop(t_philo *philo)
{
	int	stop;

	sem_wait(philo->data->dead_sem);
	stop = philo->data->simulation_stop;
	sem_post(philo->data->dead_sem);
	return (stop);
}

int	should_continue(t_philo *philo)
{
	if (should_stop(philo))
		return (0);
	if (philo->data->must_eat_count != -1
		&& philo->meals_eaten >= philo->data->must_eat_count)
		return (0);
	return (1);
}

long	calculate_odd_think_time(t_philo *philo, long safety_margin)
{
	long	think_time;

	think_time = safety_margin;
	if (think_time <= 0)
		return (0);
	if (philo->data->must_eat_count != -1)
		think_time = think_time / 4;
	else
		think_time = (think_time * 2) / 3;
	if (think_time > 600)
		think_time = 600;
	if (think_time < 1)
		think_time = 1;
	return (think_time);
}
