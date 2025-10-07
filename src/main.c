/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nistanoj <nistanoj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 14:05:26 by nistanoj          #+#    #+#             */
/*   Updated: 2025/10/07 18:35:00 by nistanoj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	join_philosophers(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->data.nb_philos)
	{
		if (pthread_join(sim->philos[i].thread, NULL) != 0)
		{
			printf("Error: Failed to join philosopher thread %d\n", i);
			return (1);
		}
		i++;
	}
	return (0);
}

static int	start_simulation(t_simulation *sim)
{
	sim->data.start_time = get_current_time();
	if (create_philosophers(sim) != 0)
		return (1);
	if (pthread_create(&sim->monitor_thread, NULL, monitor_routine, sim) != 0)
	{
		printf("Error: Failed to create monitor thread\n");
		return (1);
	}
	if (pthread_join(sim->monitor_thread, NULL) != 0)
	{
		printf("Error: Failed to join monitor thread\n");
		return (1);
	}
	if (join_philosophers(sim) != 0)
		return (1);
	return (0);
}

int	main(int argc, char **argv)
{
	t_simulation	sim;

	if (validate_args(argc, argv) != 0)
		return (1);
	if (init_simulation(&sim, argv) != 0)
		return (1);
	if (start_simulation(&sim) != 0)
		return (cleanup_simulation(&sim), 1);
	cleanup_simulation(&sim);
	return (0);
}
