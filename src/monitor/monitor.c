/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nistanoj <nistanoj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 14:07:23 by nistanoj          #+#    #+#             */
/*   Updated: 2025/11/11 01:06:18 by nistanoj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/philo.h"

static void	initial_delay(int nb_philos)
{
	if (nb_philos >= 100)
		usleep(10000);
	else
		usleep(1000);
}

static int	get_check_interval(t_simulation *sim)
{
	int	interval;

	if (sim->data.time_to_die < 50)
		interval = 100;
	else if (sim->data.time_to_die < 100)
		interval = 200;
	else if (sim->data.time_to_die < 200)
		interval = 500;
	else
		interval = 1000;
	if (sim->data.nb_philos >= 100)
		interval = interval / 2;
	return (interval);
}

void	*monitor(void *arg)
{
	t_simulation	*sim;
	int				all_ate_counter;
	int				check_interval;

	sim = (t_simulation *)arg;
	all_ate_counter = 0;
	check_interval = get_check_interval(sim);
	initial_delay(sim->data.nb_philos);
	while (1)
	{
		pthread_mutex_lock(&sim->data.stop_mutex);
		if (sim->data.simulation_stop)
		{
			pthread_mutex_unlock(&sim->data.stop_mutex);
			break ;
		}
		pthread_mutex_unlock(&sim->data.stop_mutex);
		if (monitor_checks(sim, &all_ate_counter))
			break ;
		usleep(check_interval);
	}
	return (NULL);
}
