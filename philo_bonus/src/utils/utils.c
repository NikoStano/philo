/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nistanoj <nistanoj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 22:30:00 by nistanoj          #+#    #+#             */
/*   Updated: 2025/10/26 22:14:54 by nistanoj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/philo_bonus.h"

int	ft_atoi(const char *str)
{
	int	result;
	int	i;

	result = 0;
	i = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result);
}

int	is_number(const char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

void	print_status(t_philo *philo, char *status)
{
	long	timestamp;

	sem_wait(philo->data->dead_sem);
	if (!philo->data->simulation_stop)
	{
		sem_wait(philo->data->print_sem);
		timestamp = get_current_time() - philo->data->start_time;
		printf("%ld %d %s\n", timestamp, philo->id, status);
		sem_post(philo->data->print_sem);
	}
	sem_post(philo->data->dead_sem);
}

static void	cleanup_semaphores(t_data *data)
{
	if (data->forks)
	{
		sem_close(data->forks);
		sem_unlink("/philo_forks");
	}
	if (data->print_sem)
	{
		sem_close(data->print_sem);
		sem_unlink("/philo_print");
	}
	if (data->stop_sem)
	{
		sem_close(data->stop_sem);
		sem_unlink("/philo_stop");
	}
	if (data->meal_sem)
	{
		sem_close(data->meal_sem);
		sem_unlink("/philo_meal");
	}
	if (data->dead_sem)
	{
		sem_close(data->dead_sem);
		sem_unlink("/philo_dead");
	}
}

void	cleanup_simulation(t_simulation *sim)
{
	cleanup_semaphores(&sim->data);
}
