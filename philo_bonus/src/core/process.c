/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nistanoj <nistanoj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 22:30:00 by nistanoj          #+#    #+#             */
/*   Updated: 2025/10/26 22:16:16 by nistanoj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/philo_bonus.h"

int	create_philosophers(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->data.nb_philos)
	{
		sim->pids[i] = fork();
		if (sim->pids[i] < 0)
		{
			printf("Error: Failed to fork process %d\n", i);
			return (1);
		}
		if (sim->pids[i] == 0)
		{
			philosopher_routine(&sim->philos[i]);
			exit(0);
		}
		i++;
	}
	return (0);
}
