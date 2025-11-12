/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nistanoj <nistanoj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 17:23:32 by nistanoj          #+#    #+#             */
/*   Updated: 2025/11/12 22:02:55 by nistanoj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/philo.h"

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

void	print_status(t_philo *philo, char *status)
{
	long	timestamp;
	int		should_print;

	pthread_mutex_lock(&philo->data->stop_mutex);
	should_print = !philo->data->simulation_stop;
	pthread_mutex_unlock(&philo->data->stop_mutex);
	if (!should_print)
		return ;
	pthread_mutex_lock(&philo->data->print_mutex);
	timestamp = get_current_time() - philo->data->start_time;
	printf("%ld %d %s\n", timestamp, philo->id, status);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

void	cleanup_simulation(t_simulation *sim)
{
	int	i;

	pthread_mutex_destroy(&sim->data.print_mutex);
	pthread_mutex_destroy(&sim->data.stop_mutex);
	pthread_mutex_destroy(&sim->data.meal_mutex);
	i = 0;
	while (i < sim->data.nb_philos)
	{
		pthread_mutex_destroy(&sim->forks[i]);
		i++;
	}
}
