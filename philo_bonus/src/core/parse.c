/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nistanoj <nistanoj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 22:30:00 by nistanoj          #+#    #+#             */
/*   Updated: 2025/10/26 22:15:03 by nistanoj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/philo_bonus.h"

static int	check_positive(char **argv)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		if (ft_atoi(argv[i]) <= 0)
		{
			printf("Error: All arguments must be positive integers\n");
			return (1);
		}
		i++;
	}
	return (0);
}

static int	check_numeric(char **argv)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		if (!is_number(argv[i]))
		{
			printf("Error: All arguments must be numeric\n");
			return (1);
		}
		i++;
	}
	return (0);
}

int	validate_args(int argc, char **argv)
{
	if (argc < 5 || argc > 6)
	{
		printf("Usage: %s <nb_philos> <time_to_die> ", argv[0]);
		printf("<time_to_eat> <time_to_sleep> [must_eat_count]\n");
		return (1);
	}
	if (check_numeric(argv) != 0)
		return (1);
	if (check_positive(argv) != 0)
		return (1);
	if (ft_atoi(argv[1]) > MAX_PHILOSOPHERS)
	{
		printf("Error: Maximum %d philosophers allowed\n", MAX_PHILOSOPHERS);
		return (1);
	}
	return (0);
}
