/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nistanoj <nistanoj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 18:16:09 by nistanoj          #+#    #+#             */
/*   Updated: 2025/10/17 11:01:46 by nistanoj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

/**
 * @brief Check validity of a single argument
 * @param [in] argv Argument vector
 * @param [in] i Index of the argument to check
 * @return 1 if invalid, 0 if valid
 */
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

/**
 * @brief Check the number of arguments
 * @param [in] argc Argument count
 * @param [in] argv Argument vector
 * @return 1 if invalid, 0 if valid
 */
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

/**
 * @brief Validate command-line arguments
 * @param [in] argc Argument count
 * @param [in] argv Argument vector
 * @return 1 if invalid, 0 if valid
 */
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
