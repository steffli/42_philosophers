/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stliu <stliu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 13:00:00 by stliu             #+#    #+#             */
/*   Updated: 2025/12/01 13:00:00 by stliu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

int	ft_usleep(long time, t_table *table)
{
	long	start;
	int		tmp;

	tmp = 0;
	start = get_time();
	while ((get_time() - start) < time)
	{
		usleep(100);
		tmp++;
		tmp = tmp % 50;
		pthread_mutex_lock(&table->death_lock);
		if (tmp == 0 && table->dead)
		{
			pthread_mutex_unlock(&table->death_lock);
			return (1);
		}
		pthread_mutex_unlock(&table->death_lock);
	}
	return (0);
}

int	think_time(t_philo *philo)
{
	long	time_to_think;

	time_to_think = (philo->table->time_to_die - philo->table->time_to_eat
			- philo->table->time_to_sleep) / 2;
	if (time_to_think < 0)
		time_to_think = 0;
	if (time_to_think > 600)
		time_to_think = 200;
	if (time_to_think > 0)
		return (ft_usleep(time_to_think, philo->table));
	return (0);
}

void	*one_philo_routine(t_philo *philo)
{
	print_status(philo, "has taken a fork");
	ft_usleep(philo->table->time_to_die, philo->table);
	return (NULL);
}
