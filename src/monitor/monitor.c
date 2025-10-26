/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nistanoj <nistanoj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 14:07:23 by nistanoj          #+#    #+#             */
/*   Updated: 2025/10/26 22:02:23 by nistanoj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/philo.h"

static int	get_check_interval(t_simulation *sim)
{
	int	interval;

	if (sim->data.nb_philos >= 100)
		interval = 5000;
	else if (sim->data.nb_philos >= 50)
		interval = 2000;
	else if (sim->data.nb_philos >= 10)
		interval = 1500;
	else if (sim->data.nb_philos >= 5)
		interval = 1200;
	else
		interval = 1000;
	if (sim->data.time_to_die < 200)
		interval = 500;
	return (interval);
}

static void	initial_delay(int nb_philos)
{
	if (nb_philos >= 100)
		usleep(10000);
	else
		usleep(1000);
}

void	*monitor_routine(void *arg)
{
	t_simulation	*sim;
	int				check_interval;

	sim = (t_simulation *)arg;
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
		if (check_death(sim) || check_all_ate(sim))
			break ;
		usleep(check_interval);
	}
	return (NULL);
}
