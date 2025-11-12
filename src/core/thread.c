/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nistanoj <nistanoj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 18:33:05 by nistanoj          #+#    #+#             */
/*   Updated: 2025/11/11 09:32:29 by nistanoj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/philo.h"

static int	create_thread_large(t_simulation *sim, int i, pthread_attr_t *a)
{
	if (pthread_create(&sim->philos[i].thread, a,
			philosopher_routine, &sim->philos[i]))
	{
		printf("Error: Failed to create philosopher thread %d\n", i);
		pthread_attr_destroy(a);
		return (1);
	}
	return (0);
}

static int	create_thread_small(t_simulation *sim, int i)
{
	if (pthread_create(&sim->philos[i].thread, NULL,
			philosopher_routine, &sim->philos[i]))
	{
		printf("Error: Failed to create philosopher thread %d\n", i);
		return (1);
	}
	return (0);
}

static int	init_pthread_attr(pthread_attr_t *attr)
{
	if (pthread_attr_init(attr))
		return (1);
	if (pthread_attr_setstacksize(attr, PTHREAD_STACK_MIN * 4))
	{
		pthread_attr_destroy(attr);
		return (1);
	}
	return (0);
}

int	create_philosophers(t_simulation *sim)
{
	int				i;
	pthread_attr_t	attr;

	if (sim->data.nb_philos >= 100 && init_pthread_attr(&attr) != 0)
		return (1);
	i = 0;
	while (i < sim->data.nb_philos)
	{
		if (sim->data.nb_philos >= 100)
		{
			if (create_thread_large(sim, i, &attr))
				return (1);
		}
		else
		{
			if (create_thread_small(sim, i))
				return (1);
		}
		if (sim->data.nb_philos >= 100)
			usleep(100);
		i++;
	}
	if (sim->data.nb_philos >= 100)
		pthread_attr_destroy(&attr);
	return (0);
}

int	join_philosophers(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->data.nb_philos)
	{
		if (pthread_join(sim->philos[i].thread, NULL))
		{
			printf("Error: Failed to join philosopher thread %d\n", i);
			return (1);
		}
		i++;
	}
	return (0);
}
