/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nistanoj <nistanoj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 14:05:26 by nistanoj          #+#    #+#             */
/*   Updated: 2025/11/11 01:48:36 by nistanoj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/philo.h"

static int	start_simulation(t_simulation *sim)
{
	sim->data.start_time = get_current_time();
	if (create_philosophers(sim))
		return (1);
	if (pthread_create(&sim->monitor_thread, NULL, monitor, sim))
	{
		printf("Error: Failed to create monitor thread\n");
		return (1);
	}
	if (pthread_join(sim->monitor_thread, NULL))
	{
		printf("Error: Failed to join monitor thread\n");
		return (1);
	}
	if (join_philosophers(sim))
		return (1);
	return (0);
}

int	main(int argc, char **argv)
{
	t_simulation	sim;

	if (validate_args(argc, argv))
		return (1);
	if (init_simulation(&sim, argv))
		return (1);
	if (start_simulation(&sim))
		return (cleanup_simulation(&sim), 1);
	cleanup_simulation(&sim);
	return (0);
}
