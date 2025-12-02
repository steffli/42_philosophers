/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stliu <stliu@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 14:13:13 by stliu             #+#    #+#             */
/*   Updated: 2025/12/02 12:42:35 by stliu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	init_philosophers(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->n_philos)
	{
		table->philos[i].id = i + 1;
		table->philos[i].meals_eaten = 0;
		table->philos[i].last_meal = 0;
		table->philos[i].table = table;
		table->philos[i].left_fork = &table->forks[i];
		table->philos[i].right_fork = &table->forks[(i + 1) % table->n_philos];
		i++;
	}
}

static bool	init_mutexes(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->n_philos)
	{
		if (pthread_mutex_init(&table->forks[i], NULL) != 0)
			return (false);
		i++;
	}
	if (pthread_mutex_init(&table->write_lock, NULL) != 0)
		return (false);
	if (pthread_mutex_init(&table->death_lock, NULL) != 0)
		return (false);
	return (true);
}

int	init_table(t_table *table, char **argv)
{
	table->n_philos = ft_atoi(argv[1]);
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		table->n_meals = ft_atoi(argv[5]);
	table->dead = 0;
	table->start_time = get_time();
	table->forks = malloc(sizeof(pthread_mutex_t) * table->n_philos);
	if (!table->forks)
		return (0);
	table->philos = malloc(sizeof(t_philo) * table->n_philos);
	if (!table->philos)
		return (free(table->forks), 0);
	if (!init_mutexes(table))
		return (free(table->forks), free(table->philos), 0);
	init_philosophers(table);
	return (1);
}
