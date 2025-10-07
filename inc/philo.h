/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nistanoj <nistanoj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 14:04:40 by nistanoj          #+#    #+#             */
/*   Updated: 2025/10/07 18:36:02 by nistanoj         ###   ########.fr       */
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

// main.c
int		main(int argc, char **argv);

// thread.c
int		create_philosophers(t_simulation *sim);

// init.c
int		init_simulation(t_simulation *sim, char **argv);
int		validate_args(int argc, char **argv);

// parse.c
int		validate_args(int argc, char **argv);

// utils.c
int		ft_atoi(const char *str);
int		is_number(const char *str);
void	print_status(t_philo *philo, char *status);
void	cleanup_simulation(t_simulation *sim);

// routine.c
int		take_forks(t_philo *philo);

// routine_actions.c
int		should_stop(t_philo *philo);
void	philo_eat(t_philo *philo);
void	philo_sleep(t_philo *philo);
void	philo_think(t_philo *philo);
void	philo_loop(t_philo *philo);

// routine_loop.c
void	*philosopher_routine(void *arg);

// monitor.c
void	*monitor_routine(void *arg);
int		check_death(t_simulation *sim);
int		check_all_ate(t_simulation *sim);

// time.c
long	get_current_time(void);
void	precise_usleep(long usec);
long	time_diff(long start, long end);

#endif
