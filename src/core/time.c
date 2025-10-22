/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nistanoj <nistanoj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 14:08:37 by nistanoj          #+#    #+#             */
/*   Updated: 2025/10/22 19:03:25 by nistanoj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/philo.h"

long	get_current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

long	time_diff(long start, long end)
{
	return (end - start);
}

void	precise_usleep(long usec)
{
	long	start;
	long	elapsed;
	long	remaining;

	if (usec <= 0)
		return ;
	start = get_current_time() * 1000;
	while (1)
	{
		elapsed = (get_current_time() * 1000) - start;
		remaining = usec - elapsed;
		if (remaining <= 0)
			break ;
		if (remaining > 1000)
			usleep(remaining / 2);
		else
		{
			while ((get_current_time() * 1000) - start < usec)
				usleep(10);
			break ;
		}
	}
}
