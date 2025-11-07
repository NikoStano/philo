/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nistanoj <nistanoj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 22:30:00 by nistanoj          #+#    #+#             */
/*   Updated: 2025/10/30 17:05:28 by nistanoj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <pthread.h>
# include <semaphore.h>
# include <signal.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <fcntl.h>

# define MAX_PHILOSOPHERS 200

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
	sem_t			*forks;
	sem_t			*print_sem;
	sem_t			*stop_sem;
	sem_t			*meal_sem;
	sem_t			*dead_sem;
}	t_data;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long			last_meal_time;
	t_state			state;
	pid_t			pid;
	t_data			*data;
	pthread_t		monitor_thread;
}	t_philo;

typedef struct s_simulation
{
	t_data			data;
	t_philo			philos[MAX_PHILOSOPHERS];
	pid_t			pids[MAX_PHILOSOPHERS];
}	t_simulation;

/* ************************************************************************** */
/*                          FUNCTION PROTOTYPES                               */
/* ************************************************************************** */

/* --------------------------------- Core --------------------------------- */
/* < --- init.c --- > */
int		init_simulation(t_simulation *sim, char **argv);

/* < --- parse.c --- > */
int		validate_args(int argc, char **argv);

/* < --- process.c --- > */
int		create_philosophers(t_simulation *sim);

/* < --- time.c --- > */
long	get_current_time(void);
void	precise_usleep(long usec);

/* -------------------------------- Routine --------------------------------- */
/* < --- routine_actions.c --- > */
void	philo_eat(t_philo *philo);
void	philo_sleep(t_philo *philo);
void	philo_think(t_philo *philo);
void	philo_loop(t_philo *philo);

/* < --- routine_utils.c --- > */
int		should_stop(t_philo *philo);
int		should_continue(t_philo *philo);
long	calculate_odd_think_time(t_philo *philo, long safety_margin);

/* < --- routine.c --- > */
void	philosopher_routine(t_philo *philo);

/* < --- monitor.c --- > */
void	*monitor_routine(void *arg);

/* --------------------------------- Utils ---------------------------------- */
int		ft_atoi(const char *str);
int		is_number(const char *str);
void	print_status(t_philo *philo, char *status);
void	cleanup_simulation(t_simulation *sim);

#endif
