/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stliu <stliu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 23:18:57 by stliu             #+#    #+#             */
/*   Updated: 2025/12/02 23:18:57 by stliu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	ate(t_table *table)
{
	int		i;
	bool	full;

	i = 0;
	full = true;
	if (table->n_meals == -1)
		return (false);
	while (i < table->n_philos)
	{
		pthread_mutex_lock(&table->death_lock);
		if (table->philos[i].meals_eaten < table->n_meals)
			full = false;
		pthread_mutex_unlock(&table->death_lock);
		i++;
	}
	return (full);
}

static	bool	died(t_philo *philo)
{
	long	time;
	long	start;

	start = get_time();
	pthread_mutex_lock(&philo->table->death_lock);
	time = start - philo->last_meal;
	if (time >= philo->table->time_to_die)
	{
		philo->table->dead = 1;
		pthread_mutex_unlock(&philo->table->death_lock);
		pthread_mutex_lock(&philo->table->write_lock);
		printf("%ld %d died\n",
			get_time() - philo->table->start_time, philo->id);
		pthread_mutex_unlock(&philo->table->write_lock);
		return (true);
	}
	pthread_mutex_unlock(&philo->table->death_lock);
	return (false);
}

static void	monitor(t_table *table)
{
	int	i;

	while (1)
	{
		i = 0;
		while (i < table->n_philos)
		{
			if (died(&table->philos[i]))
				return ;
			i++;
		}
		if (ate(table))
		{
			pthread_mutex_lock(&table->death_lock);
			table->dead = 1;
			pthread_mutex_unlock(&table->death_lock);
			return ;
		}
	}
}

void	start_sim(t_table *table)
{
	int	i;

	i = 0;
	table->start_time = get_time();
	while (i < table->n_philos)
	{
		table->philos[i].last_meal = get_time();
		pthread_create(&table->philos[i].thread, NULL,
			philo_routine, &table->philos[i]);
		i++;
	}
	monitor(table);
	i = 0;
	while (i < table->n_philos)
	{
		pthread_join(table->philos[i].thread, NULL);
		i++;
	}
}

void	print_status(t_philo *philo, char *state)
{
	long	time;

	pthread_mutex_lock(&philo->table->write_lock);
	pthread_mutex_lock(&philo->table->death_lock);
	if (!philo->table->dead)
	{
		time = get_time() - philo->table->start_time;
		printf("%ld %d %s\n", time, philo->id, state);
	}
	pthread_mutex_unlock(&philo->table->death_lock);
	pthread_mutex_unlock(&philo->table->write_lock);
}
