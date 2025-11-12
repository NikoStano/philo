/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nistanoj <nistanoj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 18:16:09 by nistanoj          #+#    #+#             */
/*   Updated: 2025/11/11 00:31:26 by nistanoj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/philo.h"

static int	check_argc(int argc, char **argv)
{
	if (argc < 5 || argc > 6)
	{
		printf("Usage: %s number_of_philosophers ", argv[0]);
		printf("time_to_die time_to_eat time_to_sleep ");
		printf("number_of_times_each_philosopher_must_eat\n");
		return (1);
	}
	return (0);
}

static int	is_number(const char *str)
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

static int	check_arg_validity(char **argv, int i)
{
	if (!is_number(argv[i]))
	{
		printf("Error: All arguments must be positive integers\n");
		return (1);
	}
	if (ft_atoi(argv[i]) <= 0)
	{
		printf("Error: All arguments must be positive integers\n");
		return (1);
	}
	return (0);
}

int	validate_args(int argc, char **argv)
{
	int	i;

	if (check_argc(argc, argv))
		return (1);
	i = 1;
	while (i < argc)
	{
		if (check_arg_validity(argv, i))
			return (1);
		i++;
	}
	if (ft_atoi(argv[1]) > MAX_PHILOSOPHERS)
	{
		printf("Error: Maximum %d philosophers allowed\n", MAX_PHILOSOPHERS);
		return (1);
	}
	return (0);
}
