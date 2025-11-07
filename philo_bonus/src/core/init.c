/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nistanoj <nistanoj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 22:30:00 by nistanoj          #+#    #+#             */
/*   Updated: 2025/10/26 22:15:18 by nistanoj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/philo_bonus.h"

static int	init_semaphores(t_simulation *sim)
{
	sem_unlink("/philo_forks");
	sem_unlink("/philo_print");
	sem_unlink("/philo_stop");
	sem_unlink("/philo_meal");
	sem_unlink("/philo_dead");
	sim->data.forks = sem_open("/philo_forks", O_CREAT, 0644,
			sim->data.nb_philos);
	sim->data.print_sem = sem_open("/philo_print", O_CREAT, 0644, 1);
	sim->data.stop_sem = sem_open("/philo_stop", O_CREAT, 0644, 1);
	sim->data.meal_sem = sem_open("/philo_meal", O_CREAT, 0644, 1);
	sim->data.dead_sem = sem_open("/philo_dead", O_CREAT, 0644, 1);
	if (sim->data.forks == SEM_FAILED || sim->data.print_sem == SEM_FAILED
		|| sim->data.stop_sem == SEM_FAILED || sim->data.meal_sem == SEM_FAILED
		|| sim->data.dead_sem == SEM_FAILED)
	{
		printf("Error: Failed to initialize semaphores\n");
		return (1);
	}
	return (0);
}

static void	init_philosophers(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->data.nb_philos)
	{
		sim->philos[i].id = i + 1;
		sim->philos[i].meals_eaten = 0;
		sim->philos[i].last_meal_time = 0;
		sim->philos[i].state = THINKING;
		sim->philos[i].data = &sim->data;
		sim->philos[i].pid = 0;
		i++;
	}
}

int	init_simulation(t_simulation *sim, char **argv)
{
	memset(sim, 0, sizeof(t_simulation));
	sim->data.nb_philos = ft_atoi(argv[1]);
	sim->data.time_to_die = (long)ft_atoi(argv[2]);
	sim->data.time_to_eat = (long)ft_atoi(argv[3]);
	sim->data.time_to_sleep = (long)ft_atoi(argv[4]);
	sim->data.must_eat_count = -1;
	sim->data.simulation_stop = 0;
	if (argv[5])
		sim->data.must_eat_count = ft_atoi(argv[5]);
	if (init_semaphores(sim) != 0)
		return (1);
	init_philosophers(sim);
	return (0);
}
