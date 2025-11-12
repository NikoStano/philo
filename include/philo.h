/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nistanoj <nistanoj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 14:04:40 by nistanoj          #+#    #+#             */
/*   Updated: 2025/11/13 00:40:10 by nistanoj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>

# define MAX_PHILOSOPHERS 200
# define PTHREAD_STACK_MIN 16384

typedef enum e_state
{
	THINKING,
	EATING,
	SLEEPING,
	DIED
}	t_state;

/* ************************************************************************** */
/*                              STRUCTURES                                    */
/* ************************************************************************** */

typedef struct s_data
{
	int				nb_philos;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				must_eat_count;
	long			start_time;
	int				simulation_stop;
	int				all_ate;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	stop_mutex;
	pthread_mutex_t	meal_mutex;
}	t_data;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long			last_meal_time;
	t_state			state;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_data			*data;
}	t_philo;

typedef struct s_simulation
{
	t_data			data;
	t_philo			philos[MAX_PHILOSOPHERS];
	pthread_mutex_t	forks[MAX_PHILOSOPHERS];
	pthread_t		monitor_thread;
}	t_simulation;

/* ************************************************************************** */
/*                          FUNCTION PROTOTYPES                               */
/* ************************************************************************** */
int		init_simulation(t_simulation *sim, char **argv);
int		validate_args(int argc, char **argv);
int		create_philosophers(t_simulation *sim);
int		join_philosophers(t_simulation *sim);
long	get_current_time(void);
void	precise_usleep(long usec);

/* -------------------------------- Monitor -------------------------------- */

int		monitor_checks(t_simulation *sim, int *all_ate_counter);
void	*monitor(void *arg);

/* -------------------------------- Routine -------------------------------- */
void	philo_eat(t_philo *philo);
void	philo_sleep(t_philo *philo);
void	philo_think(t_philo *philo);
void	philo_loop(t_philo *philo);
int		take_forks(t_philo *philo);
int		should_stop(t_philo *philo);
long	calculate_odd_think_time(t_philo *philo, long safety_margin);
void	*philosopher_routine(void *arg);

/* --------------------------------- Utils --------------------------------- */
int		ft_atoi(const char *str);
void	print_status(t_philo *philo, char *status);
void	cleanup_simulation(t_simulation *sim);

#endif
