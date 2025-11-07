/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nistanoj <nistanoj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 22:30:00 by nistanoj          #+#    #+#             */
/*   Updated: 2025/10/26 22:16:23 by nistanoj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/philo_bonus.h"

static void	kill_all_philosophers(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->data.nb_philos)
	{
		kill(sim->pids[i], SIGKILL);
		i++;
	}
}

static int	wait_for_philosophers(t_simulation *sim)
{
	int	i;
	int	status;
	int	death_detected;

	death_detected = 0;
	i = 0;
	while (i < sim->data.nb_philos)
	{
		waitpid(-1, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 1)
		{
			death_detected = 1;
			kill_all_philosophers(sim);
			break ;
		}
		i++;
	}
	while (i < sim->data.nb_philos)
	{
		waitpid(-1, &status, 0);
		i++;
	}
	return (death_detected);
}

static int	start_simulation(t_simulation *sim)
{
	sim->data.start_time = get_current_time();
	if (create_philosophers(sim) != 0)
		return (1);
	wait_for_philosophers(sim);
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
