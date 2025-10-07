#include "philo.h"

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

	pthread_mutex_lock(&philo->data->stop_mutex);
	if (philo->data->simulation_stop)
	{
		pthread_mutex_unlock(&philo->data->stop_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->data->stop_mutex);
	pthread_mutex_lock(&philo->data->print_mutex);
	pthread_mutex_lock(&philo->data->stop_mutex);
	if (philo->data->simulation_stop)
	{
		pthread_mutex_unlock(&philo->data->stop_mutex);
		pthread_mutex_unlock(&philo->data->print_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->data->stop_mutex);
	timestamp = get_current_time() - philo->data->start_time;
	printf("%ld %d %s\n", timestamp, philo->id, status);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

static void	destroy_mutexes(t_simulation *sim)
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

void	cleanup_simulation(t_simulation *sim)
{
	destroy_mutexes(sim);
}
