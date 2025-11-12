/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nistanoj <nistanoj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:00:00 by nistanoj          #+#    #+#             */
/*   Updated: 2025/11/11 04:32:58 by nistanoj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/philo.h"

int	should_stop(t_philo *philo)
{
	int	stop;

	pthread_mutex_lock(&philo->data->stop_mutex);
	stop = philo->data->simulation_stop;
	pthread_mutex_unlock(&philo->data->stop_mutex);
	return (stop);
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
	if (think_time > 500)
		think_time = 500;
	if (think_time < 1)
		think_time = 1;
	return (think_time);
}
